package persistence

import (
	"btcrawl/internal/persistence"
	"errors"
	"github.com/jmoiron/sqlx"
	_ "github.com/mattn/go-sqlite3"
	"github.com/stretchr/testify/assert"
	"os"
	"testing"
)

func TestPing(t *testing.T) {
	var db *sqlx.DB
	db, err := persistence.NewInMemoryDb()
	assert.NoError(t, err, "Couldn't make in  memory db")
	err = db.Ping()
	assert.NoError(t, err, "Couldn't ping to memory db")
}

func NewInMemoryTestDatabase(t *testing.T) persistence.Database {
	dbx, err := persistence.NewInMemoryDb()
	err = persistence.InjectDatabaseModel(dbx)
	assert.NoError(t, err, "Couldn't load schema")
	return persistence.NewDatabase(dbx)
}

func NewTestfileDatabase(t *testing.T) persistence.Database {
	filename := "test-database.db"
	if _, err := os.Stat(filename); !errors.Is(err, os.ErrNotExist) {
		err := os.Remove(filename)
		assert.NoError(t, err, "Failed to delete old test database")
	}

	dbx, err := persistence.NewFileDatabase(filename)
	err = persistence.InjectDatabaseModel(dbx)
	assert.NoError(t, err, "Couldn't load schema")
	return persistence.NewDatabase(dbx)
}

func TestReadDDLFromFile(t *testing.T) {
	NewInMemoryTestDatabase(t)
}

// This is essentially a smoketest to check if we can read what we want to read from the
// database.  We're not really interested in the scanner type at this point.
/*
func TestGetScannerType(t *testing.T) {
	db := NewInMemoryTestDatabase(t)

	rows, err := db.Query("SELECT id, name, description  FROM scanner_type")
	assert.NoError(t, err, "Scanner type query failed")
	// iterate over each row
	for rows.Next() {
		var id int
		// If we get any nulls, we fix that by getting sql.Nullstring
		var name sql.NullString
		var description sql.NullString
		err = rows.Scan(&id, &name, &description)
		assert.NoError(t, err, "Could not read row")
		fmt.Println("Just read ", id, ", ", name, ", ", description)
	}
}
*/
