# bluetoothsensor
Listen for presence of bluetooth and wifi devices, reporty back to the mothership.

/*
   Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleScan.cpp
   Ported to Arduino ESP32 by Evandro Copercini
   Changed to a beacon scanner to report iBeacon, EddystoneURL and EddystoneTLM beacons by beegee-tokyo
*/



TODO:

* Look over persisted reports and fix obvious errors
  * ID = -1, shouldn't happen
  * Only lowercase hexadecimal numbers, for concistency

* Make a few nice views for the stored data
* Fix naming inconsistencies between Go and SQL code
* Make makefile to test and build
* Fix gripes from goland
* Make static build that includes the sql file so that this thing
  can go from zero to sixty just from an executable
* Add logging functionality
* Add rudimentary command line parsing
* Make it run in docker (maybe)
* Make it run on raspberry pi
* Make it onto a system that just runs continously
* Refactor persistence code to be split into multiple files
* Refactor persistence code to be less repetitive


*  We're crashing due to watchdog timer timeout.  This should be simple enough to fix, so just fix it:
     D][BLEAdvertisedDevice.cpp:292] parseAdvertisement(): Type: 0x0a (), length: 1, data: 0E (18123) task_wdt: Task watchdog got triggered. The following tasks did not reset the watchdog in time:
     E (18123) task_wdt:  - IDLE0 (CPU 0)
     E (18123) task_wdt: Tasks currently running:
     E (18123) task_wdt: CPU 0: BTC_TASK
     E (18123) task_wdt: CPU 1: IDLE1  
     E (18123) task_wdt: Aborting.
     abort() was called at PC 0x401ab98c on core 0


* Order new battery powered hw (also see if it's possible to repair current).
* Make reports as complete as possible.
* Make docker or kubernetes-powered logger, logging to some searchable database
* Set up notebook access to that db (for data analysis)
* Get some enclosures, mass produce (meaning five):-)



https://en.wikipedia.org/wiki/IBeacon

