package persistence

import (
	"github.com/jmoiron/sqlx"
	_ "github.com/mattn/go-sqlite3"
	"github.com/stretchr/testify/assert"
	"testing"
)

func TestPing(t *testing.T) {
	var db *sqlx.DB
	db, err := getInMemoryDb(t)
	assert.NoError(t, err, "Couldn't make in  memory db")

	// force a connection and test that it worked
	err = db.Ping()
	assert.NoError(t, err, "Couldn't ping to memory db")

}

func getInMemoryDb(t *testing.T) (*sqlx.DB, error) {
	var db *sqlx.DB

	// exactly the same as the built-in
	db, err := sqlx.Open("sqlite3", ":memory:")
	return db, err

	// from a pre-existing sql.DB; note the required driverName
	// db, err := sqlx.NewDb(sql.Open("sqlite3", ":memory:"), "sqlite3")

}
