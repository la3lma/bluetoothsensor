--
-- When developing a new model, it's good to assume nothing
-- exists from before. Later we'll amend this.
--

DROP TABLE IF EXISTs scan_type;
DROP TABLE IF EXISTS scanner_type;
DROP TABLE IF EXISTS  scanner;
DROP TABLE IF EXISTS scan;

--
-- Table definitions
--

CREATE TABLE IF NOT EXISTS scan_type
(
  id INTEGER PRIMARY KEY AUTOINCREMENT,
  name TEXT,
  description TEXT
);
INSERT INTO scan_type(name, description)
VALUES ('bt', 'Bluetooth');

CREATE TABLE  IF NOT EXISTS scanner_type
(
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    name TEXT NOT NULL,
    description TEXT NOT NULL
);

INSERT INTO scanner_type(name, description)
VALUES ('ESP-32CAM', 'ESP-32CAM based bluetooth/ethernet scanner');

CREATE TABLE  IF NOT EXISTS scanner
(
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    mac TEXT NOT NULL,
    scannerType INTEGER NOT NULL,
    FOREIGN KEY(scannerType) REFERENCES scanner_type(id)
);

INSERT INTO scanner(mac, scannerType)
VALUES ("7C:9E:BD:4B:2F:1C", 1);

CREATE TABLE IF NOT EXISTS  scan
(
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    scannerId  INTEGER NOT NULL,
    timeOfScan INTEGER NOT NULL,
    scanType   INTEGER NOT NULL,
    ipAddress  TEXT NOT NULL,
    FOREIGN KEY(scannerId) REFERENCES scanner(id),
    FOREIGN KEY(scanType) REFERENCES  scan_type(id)
);

INSERT INTO scan(scannerId, timeofScan, scanType, ipAddress)
VALUES (1, 1, 1, "10.0.0.35");

CREATE TABLE IF NOT EXISTS  ble_report
(
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    scanId INTEGER  NOT NULL,
    bleAddress  TEXT NOT NULL,
    name TEXT,
    rssi INTEGER NOT NULL,
    haveTxPowr  INTEGER,
    serviceUUID   TEXT,
    uuid16Bit  TEXT,
    FOREIGN KEY(scanId) REFERENCES scan(id)
);

INSERT INTO ble_report(scanId, bleAddress, rssi, serviceUUID, uuid16Bit)
VALUES (1, "0b:b0:d5:e9:5d:04", -37, "0000fd6f-0000-1000-8000-00805f9b34fb", "fd6f");

CREATE TABLE IF NOT EXISTS iBeaconReport
(
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    ble_report_id INTEGER NOT NULL,
    manufacturerId INTEGER NOT NULL,
    major INTEGER NOT NULL,
    minor INTEGER NOT NULL,
    proximityUUID TEXT NOT NULL,
    power INTEGER,
    FOREIGN KEY(ble_report_id) REFERENCES ble_report(id)
);

INSERT INTO iBeaconReport(ble_report_id, manufacturerId, major, minor, proximityUUID, power)
VALUES (1, 76, 2840, 8568,  "051075e0-2737-ab65-e885-5b84a2195b00", 90);






