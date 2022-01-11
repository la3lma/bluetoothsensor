/*
   Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleScan.cpp
   Ported to Arduino ESP32 by Evandro Copercini
   Changed to a beacon scanner to report iBeacon, EddystoneURL and EddystoneTLM beacons by beegee-tokyo
*/

#include <Arduino.h>
#include <esp32-hal-log.h>
#include <esp_task_wdt.h>

// TODO: Maybe put the MDNS thing in a separate file.


#include "MyAdvertisedDeviceCallbacks.h"
#include "HttpClientAdapter.h"
#include "BtHttpServer.h"
#include "WifiConnectivity.h"
#include "MdnsSetup.h"



static const char* TAG = "Main";


int timeBetweenScans = 2000; // In milliseconds
BLEScan *pBLEScan;

BluetoothReporter *myReporter;
HttpClientAdapter *httpClientAdapter;

// TODO: Only for debugging purposes, perhaps remove?
int delayTime = 1000;

void setup()
{
  Serial.begin(115200); // TODO: Maybe set to be much faster?

  ESP_LOGV(TAG, "Starting up");
  connectToWifiNetwork();
  start_webserver();
  mdns_setup();
  esp_task_wdt_init((uint32_t) 45, true);  // TODO: Set watchdog  timeout high (45), do  raise panic on overrun
  ESP_LOGV(TAG, "Ready to serve http requests.");
}


void loop()
{
  delay(timeBetweenScans);
}
