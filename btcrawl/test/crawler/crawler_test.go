package persistence

import (
	"btcrawl/internal/crawler"
	"btcrawl/internal/persistence"
	"fmt"
	"github.com/hashicorp/mdns"
	_ "github.com/mattn/go-sqlite3"
	"github.com/stretchr/testify/assert"
	"io"
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
		close(entriesCh)
	}()

	crawler.CrawlMdnsForHttpServices("", entriesCh) // "en9
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
	for i := 1; i < 50; i++ {
		fmt.Println(fmt.Sprintf("\n\n===\nGoing for round %d", i))
		crawler.ProbeMdnsThenScanForBluetoothReports(db)
	}
}

func doTestingUsingIp(t *testing.T, ipAddr string) {
	// db := NewTestFileDatabase(t)
	db := ReuseTestFileDatabase(t)

	err := crawler.FetchBtReportFromIpAddresThenStoreInDb(ipAddr, db)
	assert.NoError(t, err)
}
