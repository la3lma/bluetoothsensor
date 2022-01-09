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
#include <WiFi.h>

#include <ArduinoJson.h>

#include "MyAdvertisedDeviceCallbacks.h"
#include "HttpClientAdapter.h"

int scanTime = 5;            //In seconds
int timeBetweenScans = 2000; // In milliseconds
BLEScan *pBLEScan;

BluetoothReporter *myReporter;

HttpClientAdapter *httpClientAdapter;

// TODO: Put in a separate file!!
void scanAndReportWifiNetworks()
{

  Serial.println("scan start");

  DynamicJsonDocument doc(20000);

  // A json object to identify this particular scanner

  String wifiMac = WiFi.macAddress();
  doc["scannerID"]["wifiMAC"] = wifiMac;

  JsonArray reports = doc.createNestedArray("wifiApReports");

  // WiFi.scanNetworks will return the number of networks found
  int n = WiFi.scanNetworks();
  Serial.println("scan done");
  if (n != 0)
  {
    for (int i = 0; i < n; ++i)
    {
      JsonObject nested = reports.createNestedObject();

      nested["SSID"] = WiFi.SSID(i);
      nested["RSSI"] = WiFi.RSSI(i);

      int encryptionType = WiFi.encryptionType(i);
      switch (encryptionType)
      {
      case WIFI_AUTH_OPEN:
        nested["encryption"] = "OPEN";
        break;

      case WIFI_AUTH_WEP:
        nested["encryption"] = "OPEN";
        break;
      case WIFI_AUTH_WPA_PSK:
        nested["encryption"] = "WPA_PSK";
        break;
      case WIFI_AUTH_WPA2_PSK:
        nested["encryption"] = "WPA2_PSK";
        break;
      case WIFI_AUTH_WPA_WPA2_PSK:
        nested["encryption"] = "WPA_WPA2_PSK";
        break;
      case WIFI_AUTH_WPA2_ENTERPRISE:
        nested["encryption"] = "WPA2_ENTERPRISE";
        break;
      case WIFI_AUTH_MAX:
        nested["encryption"] = "MAX";
        break;
      default:
        nested["encryption"] = "UNKNOWN";
        break;
      }
    }
  }

  // Print json doc.
  String json;
  serializeJsonPretty(doc, json);
  Serial.println("json doc is: ");
  Serial.println(json);
  Serial.println("Size of json doc doc is: ");
  Serial.println(json.length());

  // Send it over the wire
  httpClientAdapter->sendJsonString(json);
}

void setup()
{
  Serial.begin(115200);

  httpClientAdapter = new HttpClientAdapter("http://10.0.0.18:3000/update-sensor");

  Serial.println("Scanning...");

  myReporter = new HttpBluetoothReporter(httpClientAdapter);

  BLEDevice::init("bluetoothScanner");
  pBLEScan = BLEDevice::getScan(); //create new scan
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks(myReporter));
  pBLEScan->setActiveScan(true); //active scan uses more power, but get results faster
  pBLEScan->setInterval(100);
  pBLEScan->setWindow(99); // less or equal setInterval value
}

void loop()
{
  // put your main code here, to run repeatedly:
  myReporter->initScan(WiFi.macAddress());
  BLEScanResults foundDevices = pBLEScan->start(scanTime, false);
  Serial.print("Devices found: ");
  Serial.println(foundDevices.getCount());
  Serial.println("Scan done!");
  myReporter->scanDone();
  pBLEScan->clearResults(); // delete results fromBLEScan buffer to release memory

  scanAndReportWifiNetworks();

  delay(timeBetweenScans);
}