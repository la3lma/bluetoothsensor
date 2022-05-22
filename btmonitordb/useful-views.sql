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
    select bleAddress, scannerId, timeBucket, avg(rssi)  as 'avgRssi' from rss_series GROUP BY  timeBucket, bleAddress, scannerId ORDER BY timeBucket;

-- Objective: Use sqlite to generate  a json result that looks like this prototype
--  [{"timeBucket":5509316, "bleAddress":"0a:14:75:69:38:84"," measurements":[{"scannerId": 1, "avgRssi"}, ...]}, ...]


select * from rss_per_bucket order by timeBucket;
select * from rss_per_bucket;
select json_object('timeBucket', timeBucket, 'bleAddress', json_each.bleAddress)  from rss_per_bucket;







-- A few sample queries, just to kick the tires. Remove when done

select * from rss_per_bucket;

select * from ble_report;
select distinct(scannerId) from scan;