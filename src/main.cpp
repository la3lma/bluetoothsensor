#include <esp32-hal-log.h>
#include <esp_task_wdt.h>
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

void setup()
{
  Serial.begin(230400); // TODO: Maybe set to be much faster?

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
