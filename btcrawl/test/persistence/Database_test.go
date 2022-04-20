package persistence

import (
	"btcrawl/internal/persistence"
	"database/sql"
	"fmt"
	"github.com/jmoiron/sqlx"
	_ "github.com/mattn/go-sqlite3"
	"github.com/stretchr/testify/assert"
	"testing"
)

func TestPing(t *testing.T) {
	var db *sqlx.DB
	db, err := persistence.NewInMemoryDb(t)
	assert.NoError(t, err, "Couldn't make in  memory db")
	err = db.Ping()
	assert.NoError(t, err, "Couldn't ping to memory db")
}

func TestReadDDLFromFile(t *testing.T) {
	db, err := persistence.NewInMemoryDb(t)
	err = persistence.InjectDatabaseModel(db)
	assert.NoError(t, err, "Couldn't load schema")
}

// This is essentially a smoketest to check if we can read what we want to read from the
// database.  We're not really interested in the scanner type at this point.
func TestGetScannerType(t *testing.T) {
	db, err := persistence.NewInMemoryDb(t)
	err = persistence.InjectDatabaseModel(db)
	assert.NoError(t, err, "Couldn't load schema")

	rows, err := db.Query("SELECT id, name, description  FROM scanner_type")
	assert.NoError(t, err, "Scanner type query failed")
	// iterate over each row
	for rows.Next() {
		var id int
		// note that city can be NULL, so we use the NullString type
		var name sql.NullString
		var description sql.NullString
		err = rows.Scan(&id, &name, &description)
		assert.NoError(t, err, "Could not read row")
		fmt.Println("Just read ", id, ", ", name, ", ", description)
	}
}
