package persistence

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
