#include <esp_http_server.h>
#include <esp32-hal-log.h>
#include <WiFi.h>
#include <ArduinoJson.h>

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#include <BLEEddystoneURL.h>
#include <BLEEddystoneTLM.h>
#include <BLEBeacon.h>
#include <esp_task_wdt.h>

#include "BluetoothDeviceReportHandler.h"
#include "JsonProducingBluetoothReporter.h"
#include "MyAdvertisedDeviceCallbacks.h"

static const char* TAG = "BluetoothDeviceReportHandler";


String scanAndReportBluetoothNetwork()
{

  int scanTime = 5;            //In seconds

  ESP_LOGV(TAG, "Starting Bluetooth scan ");
  ESP_LOGV(TAG, "... scanning, this may take a while");

  JsonProducingBluetoothReporter *myReporter =  new JsonProducingBluetoothReporter();
  BLEDevice::init("bluetoothScanner");

  BLEScan *pBLEScan = BLEDevice::getScan(); //create new scan
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks(myReporter));
  pBLEScan->setActiveScan(true);
  pBLEScan->setInterval(100);
  pBLEScan->setWindow(99); // less or equal setInterval value

  myReporter->initScan();
  BLEScanResults foundDevices = pBLEScan->start(scanTime, false);
  ESP_LOGV(TAG, "Found %d bluetooth devices", foundDevices.getCount());


  String result = myReporter->scanDone();
  pBLEScan->clearResults();

  return result;
}


esp_err_t get_bluetooth_device_report_handler(httpd_req_t *req)
{
    ESP_LOGV(TAG, "get_bluetooth_device_report_handler");
    httpd_resp_set_type(req, "application/json;charset=utf-8");
    /// TODO: delete esp_task_wdt_delete(NULL); // Disable watchdog timer, this could take a while.
    httpd_resp_send(req, scanAndReportBluetoothNetwork().c_str(), HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}
