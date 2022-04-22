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
	//   ... TBD: Extensive testing of mapping actually missing.
}

func TestPersistBtReportInMemory(t *testing.T) {
	// Prepare
	err, bleScan := readTestData(t) // TODO: Convert to domain struct
	assert.NoError(t, err)
	db := NewInMemoryTestDatabase(t)

	// Act
	err = persistence.PersistBtReport(db, &bleScan)

	// Check
	assert.NoError(t, err)
}

func TestPersistBtReportInFile(t *testing.T) {
	// Prepare
	err, bleScan := readTestData(t) // TODO: Convert to domain struct
	assert.NoError(t, err)
	db := NewTestfileDatabase(t)

	// Act
	err = persistence.PersistBtReport(db, &bleScan)

	// Check
	assert.NoError(t, err)
}
