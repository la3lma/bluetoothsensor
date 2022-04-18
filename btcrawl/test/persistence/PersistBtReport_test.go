package persistence

import (
	"btcrawl/internal/persistence"
	"btcrawl/internal/report_parser"
	"github.com/stretchr/testify/assert"
	"os"
	"testing"
)

func readTestData(t *testing.T) (error, report_parser.BleScan) {
	var jsonBytes []byte
	jsonBytes, err := os.ReadFile("../testdata/example-bt-log.json")
	assert.NoError(t, err)

	bleScan, err := report_parser.ParseBtReport(jsonBytes)
	assert.NoError(t, err)
	return err, bleScan
}

func TestJsonBtoreportToDbBtReport(t *testing.T) {
	// Prepare
	err, bleScan := readTestData(t)
	assert.NoError(t, err)

	// Act
	dbObj, err := persistence.JsonBtoreportToDbBtReport(&bleScan)

	// Check
	assert.NoError(t, err)
	assert.NotEmptyf(t, dbObj, "Empty dbobject after conversion")
}

func TestPersistBtReport(t *testing.T) {
	// Prepare
	err, bleScan := readTestData(t)
	assert.NoError(t, err)

	// Act
	err = persistence.PersistBtReport(&bleScan)

	// Check
	assert.NoError(t, err)
}
