package bt_report_parser

import (
	"btcrawl/internal/bt_report_parser"
	"testing"
)

func TestHello(t *testing.T) {
	want := "Hello, world."
	if got := bt_report_parser.Parse(); got != want {
		t.Errorf("Hello() = %q, want %q", got, want)
	}
}
