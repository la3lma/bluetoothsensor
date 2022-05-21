-- This file contains view that are useful for querying
-- stored data about bluetooth (and later) other devices.
-- It is not intended to define new tables, so therefore it is ok
-- to delete everything before re-declaring it it.


DROP VIEW IF EXISTS rssi_by_bleAddress;
CREATE VIEW rssi_by_bleAddress AS
    SELECT bleAddress, scanid, s.scannerId, rssi from ble_report join scan s on ble_report.scanId = s.id order by bleAddress;

DROP VIEW IF EXISTS v1;
CREATE VIEW v1 as
select bleAddress, avg(rssi) as rssi1 from  rssi_by_bleAddress  where scannerid = 1  group by bleAddress;

DROP VIEW IF EXISTS v2;
CREATE VIEW v2 as
select bleAddress, avg(rssi) as rssi2 from  rssi_by_bleAddress  where scannerid = 2  group by bleAddress;

DROP VIEW IF EXISTS vv1;
CREATE VIEW vv1 as
select bleAddress, rssi as rssi1 from  rssi_by_bleAddress  where scannerid = 1  group by bleAddress;

DROP VIEW IF EXISTS vv2;
CREATE VIEW vv2 as
select bleAddress, rssi as rssi2 from  rssi_by_bleAddress  where scannerid = 2  group by bleAddress;

DROP VIEW IF EXISTS rss_series;
CREATE VIEW rss_series AS
    SELECT bleAddress, scanid, s.timeOfScan as 'timeOfScan', s.timeOfScan / 300 as 'timeBucket' , s.scannerId as 'scannerId',  rssi from ble_report join scan s on ble_report.scanId = s.id order by timeOfScan;

DROP VIEW IF EXISTS rss_per_bucket;
CREATE VIEW rss_per_bucket AS
    select bleAddress, scannerId, timeBucket, avg(rssi) from rss_series GROUP BY  timeBucket, bleAddress, scannerId;

-- A few sample queries, just to kick the tires. Remove when done

select * from rss_per_bucket;

select * from ble_report;
select distinct(scannerId) from scan;