/*
   Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleScan.cpp
   Ported to Arduino ESP32 by Evandro Copercini
   Changed to a beacon scanner to report iBeacon, EddystoneURL and EddystoneTLM beacons by beegee-tokyo
*/

#include <Arduino.h>


// TODO: Maybe put the MDNS thing in a separate file.


#include "MyAdvertisedDeviceCallbacks.h"
#include "HttpClientAdapter.h"
#include "BtHttpServer.h"
#include "WifiConnectivity.h"
#include "MdnsSetup.h"

#include <esp32-hal-log.h>

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
   ESP_LOGV(TAG, "Ready to serve http requests.");
}


void loop()
{
  delay(timeBetweenScans);
}
