package bt_report_parser

import (
	"btcrawl/internal/bt_report_parser"
	"testing"
)

func TestParseBtReport(t *testing.T) {
	// TODO: Read everything from this file. "../testdata/example-bt-log.json"
	want := "Hello, world."
	if got := bt_report_parser.ParseBtReport(""); got != want {
		t.Errorf("Hello() = %q, want %q", got, want)
	}
}
