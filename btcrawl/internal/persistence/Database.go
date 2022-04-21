package persistence

import (
	"github.com/jmoiron/sqlx"
	"io/ioutil"
)

type Transaction interface {
	Rollback() error
	Commit() error

	// CRUD we can do using this data model
	CreateIfNotPresent(sid ScannerID) error
	CreateBtScan(scan *BleScan) error
	CreateBleReport(bleReport *BleReport) error
	CreateIBeaconReport(iBeaconReport *IBeaconReport) error
}

type Database interface {
	BeginTransaction() (Transaction, error)
}

func NewInMemoryDb() (*sqlx.DB, error) {
	// TODO: This is an empty shell of a persistence mar db *sqlx.DB
	// exactly the same as the built-in
	db, err := sqlx.Open("sqlite3", ":memory:")
	return db, err

	// from a pre-existing sql.DB; note the required driverName
	// db, err := sqlx.NewDb(sql.Open("sqlite3", ":memory:"), "sqlite3")
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

func NewDatabase(db *sqlx.DB) Database {
	return &DbImpl{db: db}
}

type DbImpl struct {
	db *sqlx.DB
}

type TrImpl struct {
	tr *sqlx.Tx
}

func (db *DbImpl) BeginTransaction() (Transaction, error) {
	tr, err := db.db.Beginx() // TODO or beginx?
	return &TrImpl{tr: tr}, err
}

func (tr *TrImpl) CreateIfNotPresent(sid ScannerID) error {
	result, err := tr.tr.Exec(
		"INSERT INTO scan  (MAC) VALUES (?) ", // TODO: "Unless exists" thing missing.
		scan.ScannerID.Id,
		scan.TimeOfScan, scan.IpAddress)
	if err != nil {
		return err
	}

	newId, err := result.LastInsertId()
	if err != nil {
		sid.Id = newId
	}
	return err
}

func (tr *TrImpl) CreateBtScan(scan *BleScan) error { // TODO: Rename to CreateBluetoothScan
	result, err := tr.tr.Exec(
		"INSERT INTO scan  (scannerId, timeOfScan, scanType, ipAddress) VALUES (?,?,'bt',?)",
		scan.ScannerID.Id,
		scan.TimeOfScan, scan.IpAddress)
	if err != nil {
		return err
	}

	newId, err := result.LastInsertId()
	if err != nil {
		scan.Id = newId
	}
	return err
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
