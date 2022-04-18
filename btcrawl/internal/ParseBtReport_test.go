package parser

import "testing"

func TestHello(t *testing.T) {
	want := "Hello, world."
	if got := Parse(); got != want {
		t.Errorf("Hello() = %q, want %q", got, want)
	}
}
