package persistence

import (
	"fmt"
	"github.com/hashicorp/mdns"
	_ "github.com/mattn/go-sqlite3"
	"net"
	"testing"
)

func TestCrawling(t *testing.T) {
	// Make a channel for results and start listening
	entriesCh := make(chan *mdns.ServiceEntry, 4)
	go func() {
		for entry := range entriesCh {
			fmt.Printf("Got new entry: %v\n", entry)
		}
	}()

	// Start the lookup
	params := mdns.DefaultParams("_http._tcp")
	params.Entries = entriesCh
	params.DisableIPv6 = true
	params.Interface = &net.Interface{Name: "en1"}
	mdns.Query(params)
	close(entriesCh)

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
