#include <Arduino.h>


class BluetoothReporter {
  public: 
        void reportDeviceName(const char *dname);
        void reportServiceUUID(String uuid);
        void reportOBeacon(std::string strManufacturerData,  uint8_t *cManufacturerData);
        void reportIBeacon(int manufacturerId, int major, int minor, const char *proximityUUID, int signalPower);
        void initScan();
        void scanDone();
};
