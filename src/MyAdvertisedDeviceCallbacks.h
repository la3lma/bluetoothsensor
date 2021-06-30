#include <Arduino.h>

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#include <BLEEddystoneURL.h>
#include <BLEEddystoneTLM.h>
#include <BLEBeacon.h>

class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks
{
  public: 
    void onResult(BLEAdvertisedDevice advertisedDevice);
};
