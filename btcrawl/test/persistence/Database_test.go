package persistence

import (
	"btcrawl/internal/persistence"
	"github.com/jmoiron/sqlx"
	_ "github.com/mattn/go-sqlite3"
	"github.com/stretchr/testify/assert"
	"testing"
)

func TestPing(t *testing.T) {
	var db *sqlx.DB
	db, err := persistence.NewInMemoryDb(t)
	assert.NoError(t, err, "Couldn't make in  memory db")

	// force a connection and test that it worked
	err = db.Ping()
	assert.NoError(t, err, "Couldn't ping to memory db")
}

func TestReadDDLFromFile(t *testing.T) {
	db, err := persistence.NewInMemoryDb(t)
	err = persistence.InjectDatabaseModel(db)
	assert.NoError(t, err, "Couldn't load schema")
}
