package persistence

import (
	"database/sql"
	"github.com/jmoiron/sqlx"
	"io/ioutil"
	"os"
)

type Transaction interface {
	Rollback() error
	Commit() error

	// CRUD we can do using this data model
	CreateScannerIfNotPresent(sid *ScannerID) error
	CreateBtScan(scan *BleScan) error
	CreateBleReport(bleReport *BleReport) error
	CreateIBeaconReport(iBeaconReport *IBeaconReport) error
}

type Database interface {
	BeginTransaction() (Transaction, error)
}

func NewFileDatabase(filename string) (*sqlx.DB, error) {
	db, err := sql.Open("sqlite3", filename)
	if err != nil {
		return nil, err
	}
	dbx := sqlx.NewDb(db, "sqlite3")
	return dbx, nil
}

func ReuseIfExistsOrCreateAndInitializeFromSqlString(dbFilename string, schemaString string) (*sqlx.DB, error) {
	// Check if database file exists or not, and remember that fact.
	fileAlreadyExists := false
	_, err := os.Stat(dbFilename)
	if err != nil {
		fileAlreadyExists = true
	}

	// Open databasefile, creating it if it didn't already exist
	dbx, err := NewFileDatabase(dbFilename)
	if err != nil {
		return nil, err
	}

	// If database file didn't already exist, then fill it up with a full
	// schema, & we're ready to roll.
	if !fileAlreadyExists {
		err = InjectDatabaseModelFromSqlSchemaString(dbx, schemaString)
	}
	return dbx, err
}

func NewInMemoryDb() (*sqlx.DB, error) {
	// TODO: This is an empty shell of a persistence mar db *sqlx.DB
	// exactly the same as the built-in
	db, err := sqlx.Open("sqlite3", ":memory:")
	return db, err

	// from a pre-existing sql.DB; note the required driverName
	// db, err := sqlx.NewDb(sql.Open("sqlite3", ":memory:"), "sqlite3")
}

func InjectDatabaseModelFromSqlSchemaFile(db *sqlx.DB, sqlFilename string) error {
	schema, err := ioutil.ReadFile(sqlFilename)
	if err != nil {
		return err
	}
	return InjectDatabaseModelFromSqlSchemaString(db, string(schema))
}

func InjectDatabaseModelFromSqlSchemaString(db *sqlx.DB, sqlSchemaString string) error {
	_, err := db.Exec(string(sqlSchemaString))
	return err
}

func InjectDatabaseModel(db *sqlx.DB) error {
	return InjectDatabaseModelFromSqlSchemaFile(db, "../../../schema.sql")
}

func InjectProdDatabaseModel(db *sqlx.DB) error {
	schema, err := ioutil.ReadFile("../../../prodschema.sql")
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
	tr, err := db.db.Beginx()
	return &TrImpl{tr: tr}, err
}

func (tr *TrImpl) CreateScannerIfNotPresent(sid *ScannerID) error {

	var scanner []ScannerID
	err := tr.tr.Select(&scanner, "SELECT * FROM scanner  WHERE mac = ? LIMIT 1", sid.WifiMAC)
	if err != nil {
		return err
	}

	if len(scanner) == 1 {
		sid.Id = scanner[0].Id
		return nil
	}

	result, err := tr.tr.Exec(
		"INSERT INTO scanner  (mac, ScannerTypeId) VALUES (?, null) ",
		sid.WifiMAC)
	if err != nil {
		return err
	}

	newId, err := result.LastInsertId()
	if err == nil {
		sid.Id = newId
	}
	return err
}

func (tr *TrImpl) CreateBtScan(scan *BleScan) error { // TODO: Rename to CreateBluetoothScan

	err := tr.CreateScannerIfNotPresent(&scan.ScannerID)
	if err != nil {
		return err
	}

	result, err := tr.tr.Exec(
		"INSERT INTO scan(scannerId, timeOfScan, scanTypeId, ipAddress) VALUES (?,?,1,?)",
		scan.ScannerID.Id,
		scan.TimeOfScan,
		scan.IpAddress)
	if err != nil {
		return err
	}

	newId, err := result.LastInsertId()
	if err == nil {
		scan.Id = newId
	}
	return err
}

func (tr *TrImpl) CreateBleReport(bleReport *BleReport) error {
	result, err := tr.tr.Exec(
		"INSERT INTO ble_report(scanId, bleAddress, rssi, serviceUUID, uuid16bit, name) VALUES (?,?,?,?,?,?)",
		bleReport.ScanId, bleReport.BleAddress, bleReport.Rssi, bleReport.ServiceUUID, bleReport.BitUUID, bleReport.Name)
	if err != nil {
		return err
	}

	newId, err := result.LastInsertId()
	if err == nil {
		bleReport.Id = newId
	}
	return err
}

func (tr *TrImpl) CreateIBeaconReport(iBeaconReport *IBeaconReport) error {
	result, err := tr.tr.Exec(
		"INSERT INTO iBeaconReport(bleReportId, manufacturerId, major, minor, proximityUUID, power) VALUES (?,?,?,?,?,?)",
		iBeaconReport.BleReportId, iBeaconReport.ManufacturerId, iBeaconReport.Major,
		iBeaconReport.Minor, iBeaconReport.ProximityUUID, iBeaconReport.Power)
	if err != nil {
		return err
	}

	newId, err := result.LastInsertId()
	if err != nil {
		iBeaconReport.Id = newId
	}
	return err
}

func (tr *TrImpl) Rollback() error {
	return tr.tr.Rollback()
}

func (tr *TrImpl) Commit() error {
	return tr.tr.Commit()
}
