package persistence

import "btcrawl/internal/report_parser"

// TODO: Find a better way to encode null ID values than -1.

type BleScan struct {
	Id         int64     `db:"id"`
	ScannerID  ScannerID `db:"scannerId"`
	TimeOfScan int64     `db:"timeOfScan"`
	IpAddress  string    `db:"ipAddress"`
	BleReports []BleReport
}

func (s BleScan) Create() {

}

type ScannerID struct {
	Id      int64  `db:"id"`
	WifiMAC string `db:"WifiMAC"`
	// TODO: Declare the MAC as a secondary key in the DDL
}

type BleReport struct {
	Id             int64  `db:"id"`
	ScanId         int64  `db:"scanId"`
	BleAddress     string `db:"bleAddress"`
	Rssi           int    `db:"rssi"`
	ServiceUUID    string `db:"serviceUUID"`
	BitUUID        string `db:"uuid16bit"`
	IBeaconReports []IBeaconReport
}

type IBeaconReport struct {
	Id             int64  `db:"id"`
	ReportId       int64  `db:"ble_report_id"`
	ManufacturerId int    `db:"manufacturerId"`
	Major          int    `db:"major"`
	Minor          int    `db:"minor"`
	ProximityUUID  string `db:"proximityUUID"`
	Power          int    `db:"power"`
}

func JsonBtoreportToDbBtReport(scan *report_parser.BleScan) (*BleScan, error) {

	noOfBleReportsInScan := len(scan.BleReports)
	var result = BleScan{Id: -1, ScannerID: ScannerID{WifiMAC: scan.ScannerID.WifiMAC}, BleReports: make([]BleReport, noOfBleReportsInScan)}
	for i, s := range scan.BleReports {

		var noOfIbeaconReports = len(scan.BleReports[i].IBeaconReports)
		result.BleReports[i] = BleReport{
			Id:             -1,
			BleAddress:     s.BleAddress,
			Rssi:           s.Rssi,
			ServiceUUID:    s.ServiceUUID,
			BitUUID:        s.BitUUID,
			IBeaconReports: make([]IBeaconReport, noOfIbeaconReports),
		}

		for j := 0; j < noOfIbeaconReports; j++ {
			var beaconReport = s.IBeaconReports[j]
			result.BleReports[i].IBeaconReports[j] = IBeaconReport{
				Id:             -1,
				ManufacturerId: beaconReport.ManufacturerId,
				Major:          beaconReport.Major,
				Minor:          beaconReport.Minor,
				ProximityUUID:  beaconReport.ProximityUUID,
				Power:          beaconReport.Power,
			}
		}
	}
	return &result, nil
}

func persistDbBleScan(db Database, scan *BleScan) error {

	t, err := db.BeginTransaction()
	if err != err {
		return err
	}

	err = t.CreateIfNotPresent(scan.ScannerID)
	if err != nil {
		t.Rollback()
		return err
	}

	err = t.CreateBtScan(scan)
	if err != nil {
		t.Rollback()
		return err
	}

	for _, r := range scan.BleReports {
		r.ScanId = scan.Id
		err = t.CreateBleReport(&r)
		if err != nil {
			t.Rollback()
			return err
		}

		for _, b := range r.IBeaconReports {
			b.ReportId = r.Id
			err = t.CreateIBeaconReport(&b)
			if err != nil {
				t.Rollback()
				return err
			}
		}
	}

	return t.Commit()
}

func PersistBtReport(db Database, scanFromJson *report_parser.BleScan) error {
	dbBleScan, err := JsonBtoreportToDbBtReport(scanFromJson)
	if err != nil {
		return err
	}

	err = persistDbBleScan(db, dbBleScan)
	return err
}
