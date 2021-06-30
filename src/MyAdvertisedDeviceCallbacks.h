#include <Arduino.h>

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#include <BLEEddystoneURL.h>
#include <BLEEddystoneTLM.h>
#include <BLEBeacon.h>

#include "BluetoothReporter.h"
class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks
{
   BluetoothReporter *myReporter;

  public: 
    void onResult(BLEAdvertisedDevice advertisedDevice);
    MyAdvertisedDeviceCallbacks( BluetoothReporter *myReporter) {
      this->myReporter = myReporter;
    }
};
