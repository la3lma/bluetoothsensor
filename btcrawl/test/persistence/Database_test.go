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
