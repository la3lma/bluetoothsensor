
CREATE TABLE IF NOT EXISTS scan_type
(
  id INTEGER PRIMARY KEY AUTOINCREMENT,
  name TEXT,
  description TEXT
);

CREATE TABLE  IF NOT EXISTS scanner_type
(
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    name TEXT NOT NULL,
    description TEXT NOT NULL
);

CREATE TABLE  IF NOT EXISTS scanner
(
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    mac TEXT NOT NULL,
    scannerType INTEGER NOT NULL,
    FOREIGN KEY(scannerType) REFERENCES scanner_type(id)
);

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

