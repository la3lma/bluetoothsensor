package report_parser

import (
	"encoding/json"
)

type BleScan struct {
	ScannerID struct {
		WifiMAC string `json:"wifiMAC"`
	} `json:"scannerID"`
	BleReports []struct {
		BleAddress     string `json:"bleAddress"`
		Rssi           int    `json:"rssi"`
		ServiceUUID    string `json:"serviceUUID,omitempty"`
		BitUUID        string `json:"16bitUUID,omitempty"`
		IBeaconReports []struct {
			ManufacturerId int    `json:"manufacturerId"`
			Major          int    `json:"major"`
			Minor          int    `json:"minor"`
			ProximityUUID  string `json:"proximityUUID"`
			Power          int    `json:"power"`
		} `json:"iBeaconReports,omitempty"`
		HaveTXPower int    `json:"haveTXPower,omitempty"`
		Name        string `json:"name,omitempty"`
	} `json:"bleReports"`
}

func ParseBtReport(jsonBytes []byte) (BleScan, error) {
	var bleScan BleScan
	err := json.Unmarshal(jsonBytes, &bleScan)
	return bleScan, err
}
