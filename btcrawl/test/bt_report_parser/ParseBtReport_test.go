package bt_report_parser

import (
	"btcrawl/internal/bt_report_parser"
	"encoding/json"
	"fmt"
	"github.com/stretchr/testify/assert"
	"os"
	"testing"
)

func check(e error) {
	if e != nil {
		panic(e)
	}
}

func TestParseScannerId(t *testing.T) {
	scannerIdJson := `{"wifiMAC":"7C:9E:BD:4B:2F:1C"}`
	var scannerid bt_report_parser.ScannerId
	json.Unmarshal([]byte(scannerIdJson), &scannerid)
	assert.Equal(t, "7C:9E:BD:4B:2F:1C", scannerid.WifiMAC)
}

func TestParseBtReport(t *testing.T) {
	var jsonBytes []byte
	jsonBytes, err := os.ReadFile("../testdata/example-bt-log.json")
	check(err)
	// TODO: Drop this print once all the parts has been verified
	fmt.Print(string(jsonBytes))

	bleScan, err := bt_report_parser.ParseBtReport(jsonBytes)

	// TODO: Add more assertions
	assert.Equal(t, "7C:9E:BD:4B:2F:1C", bleScan.ScannerID.WifiMAC)
}
