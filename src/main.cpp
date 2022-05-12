#include <esp32-hal-log.h>
#include <esp_task_wdt.h>
#include "MyAdvertisedDeviceCallbacks.h"
#include "BtHttpServer.h"
#include "WifiConnectivity.h"
#include "MdnsSetup.h"

static const char* TAG = "Main";

int millisecondsBetweenResets = 1000 * 60 * 7; // In milliseconds
BLEScan *pBLEScan;

BluetoothReporter *myReporter;

void setup()
{
  Serial.begin(230400); // TODO: Maybe set to be much faster?

  ESP_LOGV(TAG, "Starting up");
  connectToWifiNetwork();
  start_webserver();
  mdns_setup();
  esp_task_wdt_init((uint32_t) 45, true);  // Set watchdog  timeout high (45), do raise panic on overrun
  // TODO: Explore what happens with the watchdog timers, trap errors and report
  // situation.  Maybe use this trick? https://www.esp32.com/viewtopic.php?t=7583
  ESP_LOGV(TAG, "Ready to serve http requests.");
}


void loop()
{
  // There are bugs that makes the current implementation freeeze up after
  // serving a couple (or so) of requests.  This is a hack to reset the
  // service every few minutes so that it doesn't continue hanging 
  // without responding.
  delay(millisecondsBetweenResets);
  ESP.restart();
}
