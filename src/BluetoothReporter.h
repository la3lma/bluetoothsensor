#ifndef BluetoothReporter_h
#define BluetoothReporter_h

#include <Arduino.h>
#include <ArduinoJson.h>

#include "HttpClientAdapter.h"

#include <list>
#include <map>

// TODO: Refactor this stuff.
struct DeviceNameReport
{
    char *deviceName;
};

struct ServiceUuidReport
{
    char *uuid;
};

struct IBeaconReport
{
    std::string proximityUuid;
    int major;
    int minor;
    int signalPower;
    int manufacturerId;
};

class BLEBasicReport final
{
public:
    std::string bleAddress;
    uint16_t appearance;
    std::string manufacturerData;
    std::string name;
    std::string serviceUUID;
    std::string serviceUUIDVendorCode;
    int rssi;
    int8_t txPower;

    std::list<IBeaconReport *> iBeaconReports;

    /*   TODO: These are the parts we're not reporting yet, because we 
        don't know how to 
	BLEScan*    getScan();
	std::string getServiceData();
	std::string getServiceData(int i);

	BLEUUID     getServiceDataUUID();
	BLEUUID     getServiceDataUUID(int i);
	BLEUUID     getServiceUUID();
	BLEUUID     getServiceUUID(int i);

	int         getServiceDataCount;
	int         getServiceDataUUIDCount;
	int         getServiceUUIDCount;

	uint8_t* 	getPayload();
	size_t		getPayloadLength();

    */
    bool haveAppearance;
    bool haveManufacturerData;
    bool haveName;
    bool haveRSSI;
    bool haveServiceData;
    bool haveServiceUUID;
    bool haveTXPower;

    void toJson(JsonObject json);
};

class BluetoothReporter
{
public:
    virtual void initScan(String wifiMAC);
    virtual void scanDone();

    virtual bool hasKey(std::string &bleAddress);
    virtual BLEBasicReport * registerNewReport(std::string bleAddress);
};

class HttpBluetoothReporter final : public BluetoothReporter
{
private:
    HttpClientAdapter *httpClientAdapter;
    std::map<std::string, BLEBasicReport *> reports;
    std::string wifiMAC;

public:
    HttpBluetoothReporter(HttpClientAdapter *httpClientAdapter);
    void initScan(String wifiMAC);
    void scanDone();

    bool hasKey(std::string &bleAddress);
    BLEBasicReport * registerNewReport(std::string bleAddress);
};

char *safeCopy(const char *arg);

char *safeStringCopy(std::string arg);


#endif
