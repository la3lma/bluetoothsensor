package bt_report_parser

import (
	"btcrawl/internal/bt_report_parser"
	"fmt"
	"os"
	"testing"
)

func check(e error) {
	if e != nil {
		panic(e)
	}
}

func TestParseBtReport(t *testing.T) {
	json, err := os.ReadFile("../testdata/example-bt-log.json")
	check(err)
	fmt.Print(string(json))

	want := "Hello, world."
	if got := bt_report_parser.ParseBtReport(string(json)); got != want {
		t.Errorf("Hello() = %q, want %q", got, want)
	}
}
