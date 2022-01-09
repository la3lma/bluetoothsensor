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
// TODO: Maybe put the MDNS thing in a separate file.
#include <WiFi.h>

#include <ArduinoJson.h>

#include "MyAdvertisedDeviceCallbacks.h"
#include "HttpClientAdapter.h"
#include "BtHttpServer.h"
#include "WifiConnectivity.h"
#include "MdnsSetup.h"

#include <esp32-hal-log.h>

static const char* TAG = "Main";

int scanTime = 5;            //In seconds
int timeBetweenScans = 2000; // In milliseconds
BLEScan *pBLEScan;

BluetoothReporter *myReporter;

HttpClientAdapter *httpClientAdapter;



// TODO: Only for debugging purposes, perhaps remove?
int delayTime = 1000;



// TODO: Take a long hard look at  https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/protocols/esp_http_server.html


void setup()
{
  Serial.begin(115200);

  ESP_LOGV(TAG, "running setup");
  connectToWifiNetwork();
  

/*
  httpClientAdapter = new HttpClientAdapter("http://10.0.0.18:3000/update-sensor");


  ESP_LOGV(TAG, "... scanning");

  myReporter = new HttpBluetoothReporter(httpClientAdapter);

  BLEDevice::init("bluetoothScanner");
  pBLEScan = BLEDevice::getScan(); //create new scan
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks(myReporter));
  pBLEScan->setActiveScan(true); //active scan uses more power, but get results faster
  pBLEScan->setInterval(100);
  pBLEScan->setWindow(99); // less or equal setInterval value
  */

  start_webserver();
  mdns_setup();
}



void loop()
{
  /*
  // put your main code here, to run repeatedly:
  myReporter->initScan(WiFi.macAddress());
  BLEScanResults foundDevices = pBLEScan->start(scanTime, false);
  ESP_LOGV(TAG, "Found %d bluetooth devices", foundDevices.getCount());
  myReporter->scanDone();
  pBLEScan->clearResults(); // delete results fromBLEScan buffer to release memory

  // scanAndReportWifiNetworks();
  */

  delay(timeBetweenScans);
}
