package persistence

import (
	"github.com/jmoiron/sqlx"
	"io/ioutil"
	"testing"
)

type Transaction interface {
	Rollback() error
	Commit() error

	// CRUD we can do using this data model
	CreateScan(scan *BleScan) error
	CreateBleReport(bleReport *BleReport) error
	CreateIBeaconReport(iBeaconReport *IBeaconReport) error
}

type Database interface {
	BeginTransaction() Transaction
}

// TODO: This is an empty shell of a persistence model

func InjectDatabaseModel(db *sqlx.DB) error {
	schema, err := ioutil.ReadFile("../../../schema.sql")
	if err != nil {
		return err
	}
	_, err = db.Exec(string(schema))
	return err
}

func NewInMemoryDb(t *testing.T) (*sqlx.DB, error) {
	var db *sqlx.DB

	// exactly the same as the built-in
	db, err := sqlx.Open("sqlite3", ":memory:")
	return db, err

	// from a pre-existing sql.DB; note the required driverName
	// db, err := sqlx.NewDb(sql.Open("sqlite3", ":memory:"), "sqlite3")
}

type DbImpl struct {
}

type TrImpl struct {
}

func (db *DbImpl) BeginTransaction() Transaction {
	return &TrImpl{}
}

func (db *TrImpl) CreateScan(scan *BleScan) error {
	return nil
}

func (db *TrImpl) CreateBleReport(bleReport *BleReport) error {
	return nil
}

func (db *TrImpl) CreateIBeaconReport(iBeaconReport *IBeaconReport) error {
	return nil
}

func (db *TrImpl) Rollback() error {
	return nil
}

func (db *TrImpl) Commit() error {
	return nil
}
