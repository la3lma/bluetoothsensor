package persistence

import (
	"btcrawl/internal/crawler"
	"btcrawl/internal/persistence"
	"btcrawl/internal/report_parser"
	"fmt"
	"github.com/hashicorp/mdns"
	_ "github.com/mattn/go-sqlite3"
	"github.com/stretchr/testify/assert"
	"io"
	"net"
	"net/http"
	"strings"
	"testing"
	"time"
)

func FetchBtUsageReport(url string) {

	client := http.Client{
		Timeout: 450 * time.Second, // TODO: Set to 45 or 20
	}

	fmt.Println("Zot")
	resp, err := client.Get(url)
	fmt.Println("Pop")
	if err != nil {
		fmt.Println("Error while receiving ", err)
		return
	}

	body, err := io.ReadAll(resp.Body)
	if err != nil {
		fmt.Println("Error while receiving ", err)
		resp.Body.Close()
		return
	}
	fmt.Printf("Foo => %d\n", len(body))
	fmt.Println("Done")
	resp.Body.Close()
}

func GetBluetoothScanningUrlFromEntry(entry *mdns.ServiceEntry) string {
	url := fmt.Sprintf("http://%s/", entry.AddrV4)

	if strings.HasPrefix(entry.Name, "btmonitor-") { // TODO: Magic string removal
		// This is a  bluetooth monitor instance, poll it
		fmt.Println("Got a btmonitor, fetching content")
		url = fmt.Sprintf("http://%s/bluetooth-device-report", entry.AddrV4)
		return url
	} else {
		return ""
	}
}

func HandleMdnsEntry(entry *mdns.ServiceEntry) {
	url := fmt.Sprintf("http://%s/", entry.AddrV4)

	if strings.HasPrefix(entry.Name, "btmonitor-") { // TODO: Magic string removal
		// This is a  bluetooth monitor instance, poll it
		fmt.Println("Got a btmonitor, fetching content")
		url = fmt.Sprintf("http://%s/bluetooth-device-report", entry.AddrV4)
	} else {
		return
	}

	fmt.Printf("Got new entry: %v\n", entry)

	fmt.Println("Name = ", entry.Name)
	fmt.Println("ipv4 = ", entry.AddrV4)
	fmt.Println("URL = ", url)

	FetchBtUsageReport(url)

	fmt.Println("nobody")
}

// https://pkg.go.dev/github.com/hashicorp/mdns#section-readme

func CrawlMdnsForHttpServices(device string, consumer chan *mdns.ServiceEntry) {
	// Start the lookup, ipv4 only
	params := mdns.DefaultParams("_http._tcp")
	params.Entries = consumer
	params.DisableIPv6 = true
	params.Interface = &net.Interface{Name: "en9"}
	mdns.Query(params)
	fmt.Println("trubadurix")
	close(entriesCh)
	fmt.Println("Business")
}

func TestMdnsCrawling(t *testing.T) {
	// Make a channel for results and start listening, only carries
	// four requests in flight at the same time

	maxRequestsInFlight := 4
	entriesCh := make(chan *mdns.ServiceEntry, maxRequestsInFlight)

	// Setting up a consumer running in parallel
	go func() {
		for entry := range entriesCh {
			HandleMdnsEntry(entry)
			fmt.Println("foam")
		}
	}()

	CrawlMdnsForHttpServices("en9", entriesCh)

}

func NewInMemoryTestDatabase(t *testing.T) persistence.Database {
	dbx, err := persistence.NewInMemoryDb()
	err = persistence.InjectDatabaseModel(dbx)
	assert.NoError(t, err, "Couldn't load schema")
	return persistence.NewDatabase(dbx)
}

func ReuseTestFileDatabase(t *testing.T) persistence.Database {
	dbx, err := persistence.ReuseIfExistsOrCreateAndInitialize("full-tilt-scan-database.db")
	assert.NoError(t, err, "Couldn't load schema")
	return persistence.NewDatabase(dbx)
}

// TODO: 1. Fix problem with bad scan number (injects two distinct scans, but only one should be injected)
//       2. Fix problem with database truncating everything in it from previously.

func TestSingleFetchEndToEnd(t *testing.T) {

	ipAddrs := [2]string{"10.0.0.36", "10.0.0.35"}
	for x := 0; x < 1000; x++ {
		ipnr := ipAddrs[x%len(ipAddrs)]
		fmt.Println("RUnning test # ", x, " on ip ", ipnr)
		doTestingUsingIp(t, ipnr)
	}
}

func TestScanFromMdnsToDatabase(t *testing.T) {

	db := ReuseTestFileDatabase(t)
	maxRequestsInFlight := 4
	entriesCh := make(chan *mdns.ServiceEntry, maxRequestsInFlight)

	// Setting up a consumer running in parallel
	go func() {
		for entry := range entriesCh {
			url := GetBluetoothScanningUrlFromEntry(entry)
			if len(url) < 1 {
				continue
			}

			// TODO: Do the scan, then stash in db
			FetchBtReportFromUrlThenStoreInDb(url, db)
		}
	}()

	CrawlMdnsForHttpServices("en9", entriesCh)
}

func FetchBtReportFromIpAddresThenStoreInDb(ipAddr string, db Database) error {
	url := fmt.Sprintf("http://%s/bluetooth-device-report", ipAddr)
	FetchBtReportFromIpAddresThenStoreInDb(url, db)
}

func FetchBtReportFromUrlThenStoreInDb(url string, db Database) error {

	jsonBytes, err := crawler.GetBluetoothReportStringFromScanner(url)
	assert.NoError(t, err)

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

func doTestingUsingIp(t *testing.T, ipAddr string) {
	// db := NewTestFileDatabase(t)
	db := ReuseTestFileDatabase(t)
	err := FetchBtReportFromIpAddresThenStoreInDb(t, string)
	assert.NoError(t, err)
}
