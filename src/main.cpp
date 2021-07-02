/*
   Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleScan.cpp
   Ported to Arduino ESP32 by Evandro Copercini
   Changed to a beacon scanner to report iBeacon, EddystoneURL and EddystoneTLM beacons by beegee-tokyo
*/

#include <Arduino.h>

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#include <BLEEddystoneURL.h>
#include <BLEEddystoneTLM.h>
#include <BLEBeacon.h>

#include "MyAdvertisedDeviceCallbacks.h"

int scanTime = 5; //In seconds
BLEScan *pBLEScan;

BluetoothReporter *myReporter;

void setup()
{
  Serial.begin(115200);
  Serial.println("Scanning...");

  // TODO: This is now a reporter that reports to standard out.  That is a bit silly, should
  //       instead report to a service on the intertubes.  We'll get there eventually.
  myReporter = new HttpBluetoothReporter;

  BLEDevice::init("");
  pBLEScan = BLEDevice::getScan(); //create new scan
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks(myReporter));
  pBLEScan->setActiveScan(true); //active scan uses more power, but get results faster
  pBLEScan->setInterval(100);
  pBLEScan->setWindow(99); // less or equal setInterval value
}

void loop()
{
  // put your main code here, to run repeatedly:
  myReporter->initScan();
  BLEScanResults foundDevices = pBLEScan->start(scanTime, false);
  Serial.print("Devices found: ");
  Serial.println(foundDevices.getCount());
  Serial.println("Scan done!");
  myReporter->scanDone();
  pBLEScan->clearResults(); // delete results fromBLEScan buffer to release memory
  delay(2000);
}