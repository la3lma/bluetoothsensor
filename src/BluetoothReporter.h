#include <Arduino.h>
#include "HttpClientAdapter.h"

#include<list>

class BluetoothReporter
{
public:
    virtual void reportDeviceName(const char *dname);
    virtual void reportServiceUUID(String uuid);
    virtual void reportOBeacon(std::string strManufacturerData, uint8_t *cManufacturerData);
    virtual void reportIBeacon(int manufacturerId, int major, int minor, const char *proximityUUID, int signalPower);
    virtual void initScan();
    virtual void scanDone();
};

class SerialBluetoothReporter final : public BluetoothReporter
{
public:
    void reportDeviceName(const char *dname);
    void reportServiceUUID(String uuid);
    void reportOBeacon(std::string strManufacturerData, uint8_t *cManufacturerData);
    void reportIBeacon(int manufacturerId, int major, int minor, const char *proximityUUID, int signalPower);
    void initScan();
    void scanDone();
};


class HttpBluetoothReporter final : public BluetoothReporter
{
private:
    HttpClientAdapter *httpClientAdapter;
    std::list<String>  things;

public:
    HttpBluetoothReporter(HttpClientAdapter *httpClientAdapter);
    void reportDeviceName(const char *dname);
    void reportServiceUUID(String uuid);
    void reportOBeacon(std::string strManufacturerData, uint8_t *cManufacturerData);
    void reportIBeacon(int manufacturerId, int major, int minor, const char *proximityUUID, int signalPower);
    void initScan();
    void scanDone();
};
