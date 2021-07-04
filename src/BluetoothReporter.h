#include <Arduino.h>
#include "HttpClientAdapter.h"

#include<list>

class BluetoothReporter
{
public:
    virtual void reportDeviceName(const char *dname);
    virtual void reportServiceUUID(const char * uuid);
    virtual void reportOBeacon(std::string strManufacturerData, uint8_t *cManufacturerData);
    virtual void reportIBeacon(int manufacturerId, int major, int minor, const char *proximityUUID, int signalPower);
    virtual void initScan();
    virtual void scanDone();
};

class SerialBluetoothReporter final : public BluetoothReporter
{
public:
    void reportDeviceName(const char *dname);
    void reportServiceUUID(const char *uuid);
    void reportOBeacon(std::string strManufacturerData, uint8_t *cManufacturerData);
    void reportIBeacon(int manufacturerId, int major, int minor, const char *proximityUUID, int signalPower);
    void initScan();
    void scanDone();
};


// TODO: Refactor this stuff.
struct DeviceNameReport {
    char *deviceName;
};

struct ServiceUuidReport {
    char *uuid;
};

class HttpBluetoothReporter final : public BluetoothReporter
{
private:
    HttpClientAdapter *httpClientAdapter;
    std::list<DeviceNameReport   *>  deviceNameReports;
    std::list<ServiceUuidReport  *>  serviceUuidReports;

public:
    HttpBluetoothReporter(HttpClientAdapter *httpClientAdapter);
    void reportDeviceName(const char *dname);
    void reportServiceUUID(const char * uuid);
    void reportOBeacon(std::string strManufacturerData, uint8_t *cManufacturerData);
    void reportIBeacon(int manufacturerId, int major, int minor, const char *proximityUUID, int signalPower);
    void initScan();
    void scanDone();
};
