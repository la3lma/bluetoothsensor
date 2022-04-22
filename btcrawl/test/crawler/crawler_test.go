package persistence

import (
	_ "github.com/mattn/go-sqlite3"
	"testing"
)

func NewCrawler() (MdnsCrawler, error) {
	return nil, nil
}

func TestCrawling(t *testing.T) {
	c, err := NewCrawler()
	if err != nil {
		return nil, err
	}

	// https://pkg.go.dev/github.com/hashicorp/mdns#section-readme
	// Scan MDNS for stuff that is present
	// Keep it in a cache maybe?
	// Then crawl throug everything in the list of known thingies,
	//   remove those who don't answer
	// Send the results to persistence (use a channel maybe?)
	// Tha's it.

	// Usage:   btcrawl database.db

}
