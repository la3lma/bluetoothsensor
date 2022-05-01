package crawler

import (
	"btcrawl/internal/persistence"
	"btcrawl/internal/report_parser"
	"fmt"
	"github.com/hashicorp/mdns"
	"io"
	"net"
	"net/http"
	"strings"
	"sync"
	"time"
)

func GetBluetoothReportStringFromScanner(url string) ([]byte, error) {

	client := http.Client{
		Timeout: 25 * time.Second, // TODO: Set to something in the interval 45 to  20 for prod
	}

	resp, err := client.Get(url)
	if err != nil {
		return []byte{}, err
	}

	body, err := io.ReadAll(resp.Body)
	defer resp.Body.Close()
	if err != nil {
		return []byte{}, err
	}

	return body, err
}

func FetchBtReportFromUrlThenStoreInDb(ipAddr string, url string, db persistence.Database) error {

	jsonBytes, err := GetBluetoothReportStringFromScanner(url)
	if err != nil {
		return err
	}

	if len(jsonBytes) < 10 {
		fmt.Println("No bytes read")
		return nil
	}

	bleScan, err := report_parser.ParseBtReport(jsonBytes)
	if err != nil {
		return err
	}

	dbObj, err := persistence.JsonBtoreportToDbBtReport(&bleScan)
	if err != nil {
		return err
	}

	dbObj.IpAddress = ipAddr
	dbObj.TimeOfScan = time.Now().Unix()

	err = persistence.PersistDbBleScan(db, dbObj)
	if err != nil {
		return err
	}

	fmt.Println("Done, scan recorded")
	return nil
}

func FetchBtReportFromIpAddresThenStoreInDb(ipAddr string, db persistence.Database) error {
	url := fmt.Sprintf("http://%s/bluetooth-device-report", ipAddr)
	return FetchBtReportFromUrlThenStoreInDb(ipAddr, url, db)
}

// https://pkg.go.dev/github.com/hashicorp/mdns#section-readme

func CrawlMdnsForHttpServices(device string, consumer chan *mdns.ServiceEntry) {
	// Start the lookup, ipv4 only
	params := mdns.DefaultParams("_http._tcp")
	params.Entries = consumer
	params.DisableIPv6 = true
	params.Timeout = time.Duration(15000000000) // 15 seconds in nanoseconds
	if len(device) > 0 {
		params.Interface = &net.Interface{Name: device}
	}
	mdns.Query(params)
	close(consumer)
}

func GetBluetoothScanningUrlFromEntry(entry *mdns.ServiceEntry) (string, string) {
	ip := fmt.Sprintf("%s", entry.AddrV4)
	url := ""
	if strings.HasPrefix(entry.Name, "btmonitor-") { // TODO: Magic string removal
		// This is a  bluetooth monitor instance, poll it
		fmt.Println("Got a btmonitor, fetching content")
		url = fmt.Sprintf("http://%s/bluetooth-device-report", entry.AddrV4)
	}

	return ip, url
}

func ProbeMdnsThenScanForBluetoothReports(db persistence.Database) {
	maxRequestsInFlight := 4
	var wg *sync.WaitGroup = new(sync.WaitGroup)
	wg.Add(1)
	entriesCh := make(chan *mdns.ServiceEntry, maxRequestsInFlight)

	// Setting up a consumer running in parallel
	go func() {
		defer wg.Done()
		for entry := range entriesCh {
			ipAddr, url := GetBluetoothScanningUrlFromEntry(entry)
			fmt.Println(fmt.Sprintf("Considering url = '%s'.  ipAddr = %s", url, ipAddr))
			if len(url) < 1 {
				fmt.Println("  ... skipping")
				continue
			}

			// TODO: Do the scan, then stash in db
			fmt.Println("About to fetch  from ", url)
			FetchBtReportFromUrlThenStoreInDb(ipAddr, url, db)
			fmt.Println("Done fetching from ", url)
		}
		fmt.Println("One round of crawling done.")
	}()

	fmt.Println("Start crawling")
	CrawlMdnsForHttpServices("en9", entriesCh)
	wg.Wait()
	fmt.Println("Done crawling")
}
