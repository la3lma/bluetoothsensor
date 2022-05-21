--
-- When developing a new model, it's good to assume nothing
-- exists from before. Later we'll amend this.
--

DROP TABLE IF EXISTs scan_type;
DROP TABLE IF EXISTS scanner_type;
DROP TABLE IF EXISTS scanner;
DROP TABLE IF EXISTS scan;
DROP TABLE IF EXISTS ble_report;
DROP TABLE IF EXISTS iBeaconReport;
DROP TABLE IF EXISTS uuid_16_bit;
