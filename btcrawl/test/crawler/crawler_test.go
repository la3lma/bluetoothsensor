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

func fetchBtUsageReport(url string) {

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

func HandleEntry(entry *mdns.ServiceEntry) {
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

	fetchBtUsageReport(url)

	fmt.Println("nobody")
}

func TestCrawling(t *testing.T) {
	// Make a channel for results and start listening
	entriesCh := make(chan *mdns.ServiceEntry, 4)
	go func() {
		for entry := range entriesCh {
			HandleEntry(entry)
			fmt.Println("foam")
		}
	}()

	// Start the lookup, ipv4 only
	params := mdns.DefaultParams("_http._tcp")
	params.Entries = entriesCh
	params.DisableIPv6 = true
	params.Interface = &net.Interface{Name: "en9"}
	for i := 1; i < 5; i++ {
		mdns.Query(params)
		fmt.Println("trubadurix")
	}
	close(entriesCh)
	fmt.Println("Business")

	// https://pkg.go.dev/github.com/hashicorp/mdns#section-readme
	// Scan MDNS for stuff that is present
	// Keep it in a cache maybe?
	// Then crawl throug everything in the list of known thingies,
	//   remove those who don't answer
	// Send the results to persistence (use a channel maybe?)
	// Tha's it.
	// Need to explicitly state the interface to use for this thing to work. Maybe I should just iterate over all the en interfaces and catch them all?  ... or at least have a list of them as command line parameters

	// Usage:   btcrawl database.db

}

func NewInMemoryTestDatabase(t *testing.T) persistence.Database {
	dbx, err := persistence.NewInMemoryDb()
	err = persistence.InjectDatabaseModel(dbx)
	assert.NoError(t, err, "Couldn't load schema")
	return persistence.NewDatabase(dbx)
}

func ReuseTestFileDatabase(t *testing.T) persistence.Database {
	dbx, err := persistence.Reuse("full-tilt-scan-database.db")
	// TODO: Only inject if the database doesn't already exist from before.
	err = persistence.InjectProdDatabaseModel(dbx)
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

func doTestingUsingIp(t *testing.T, ipAddr string) {
	// db := NewTestFileDatabase(t)
	db := ReuseTestFileDatabase(t)

	url := fmt.Sprintf("http://%s/bluetooth-device-report", ipAddr)

	jsonBytes, err := crawler.GetBluetoothReportStringFromScanner(url)
	assert.NoError(t, err)

	if len(jsonBytes) < 10 {
		fmt.Println("No bytes read")
		return
	}

	bleScan, err := report_parser.ParseBtReport(jsonBytes)
	assert.NoError(t, err)

	dbObj, err := persistence.JsonBtoreportToDbBtReport(&bleScan)
	dbObj.IpAddress = ipAddr
	dbObj.TimeOfScan = time.Now().Unix()
	// TODO: Bug: There are two scan IDs recorded, but there should be only one.
	err = persistence.PersistDbBleScan(db, dbObj)

	assert.NoError(t, err, "Couldn't store in database: ", err)
	fmt.Println("Done, scan recorded")
}
