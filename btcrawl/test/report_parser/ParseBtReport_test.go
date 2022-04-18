package report_parser

import (
	"btcrawl/internal/report_parser"
	"github.com/stretchr/testify/assert"
	"os"
	"testing"
)

func TestParseBtReport(t *testing.T) {
	var jsonBytes []byte
	jsonBytes, err := os.ReadFile("../testdata/example-bt-log.json")
	assert.NoError(t, err)

	bleScan, err := report_parser.ParseBtReport(jsonBytes)
	assert.NoError(t, err)

	// TODO: Add more assertions, ... or not
	assert.Equal(t, "7C:9E:BD:4B:2F:1C", bleScan.ScannerID.WifiMAC)
	assert.Equal(t, -37, bleScan.BleReports[0].Rssi)
}
