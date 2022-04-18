package bt_report_parser

import (
	"encoding/json"
	"fmt"
)

// {"scannerID":{"wifiMAC":"7C:9E:BD:4B:2F:1C"},"bleReports":[{"bleAddress":"0b:b0:d5:e9:5d:04","rssi":-37,"serviceUUID":"0000fd6f-0000-1000-8000-00805f9b34fb","16bitUUID":"fd6f"},{"bleAddress":"23:37:99:c1:d9:d5","rssi":-47},{"bleAddress":"30:30:36:96:8c:49","rssi":-83},{"bleAddress":"47:98:c1:17:56:77","rssi":-46,"iBeaconReports":[{"manufacturerId":76,"major":2840,"minor":8568,"proximityUUID":"051075e0-2737-ab65-e885-5b84a2195b00","power":90}]},{"bleAddress":"54:71:cb:6c:df:97","rssi":-45,"iBeaconReports":[{"manufacturerId":76,"major":2840,"minor":8568,"proximityUUID":"051075e0-2737-ab65-e885-5b84a2195b00","power":90}]},{"bleAddress":"5a:68:3c:b3:f5:1a","rssi":-64,"haveTXPower":4},{"bleAddress":"60:0d:cb:af:e4:4b","rssi":-34,"haveTXPower":7},{"bleAddress":"61:84:96:2e:29:86","name":"LE-Dopsie","rssi":-58,"serviceUUID":"0000febe-0000-1000-8000-00805f9b34fb","16bitUUID":"febe","haveTXPower":-10},{"bleAddress":"63:93:db:29:c9:f8","rssi":-90},{"bleAddress":"c6:67:c8:f8:0c:86","rssi":-50},{"bleAddress":"c8:c1:a8:a7:63:72","name":"Z_7_6031705","rssi":-59,"serviceUUID":"0000de00-3dd4-4255-8d62-6dc7b9bd5561","16bitUUID":"de00","haveTXPower":12},{"bleAddress":"c9:70:53:86:bc:62","rssi":-93,"haveTXPower":-21},{"bleAddress":"ec:77:80:b7:cc:5f","rssi":-36}]}
type ScannerId struct { // TODO: Begone
	WifiMAC string `json:"wif iMAC"`
}

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
	fmt.Print(string(jsonBytes))
	var bleScan BleScan
	json.Unmarshal(jsonBytes, &bleScan)
	return bleScan, nil
}
