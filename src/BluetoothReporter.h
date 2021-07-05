#ifndef BluetoothReporter_h
#define BluetoothReporter_h


#include <Arduino.h>
#include <ArduinoJson.h>

#include "HttpClientAdapter.h"

#include<list>
#include<map>


// TODO: Refactor this stuff.
struct DeviceNameReport {
    char *deviceName;
};

struct ServiceUuidReport {
    char *uuid;
};

struct IBeaconReport {
    char *proximityUuid;
    int major;
    int minor;
    int signalPower;
    int manufacturerId;
};


class BLEBasicReport final {
    public:
    std::string bleAddress;
    uint16_t appearance;
    std::string manufacturerData;
    std::string name;
    std::string serviceUUID;
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
    virtual void initScan();
    virtual void scanDone();
    virtual void bluBasicReport(BLEBasicReport *report);
};

class HttpBluetoothReporter final : public BluetoothReporter
{
private:
    HttpClientAdapter                 *httpClientAdapter;
    std::list<BLEBasicReport      *>  reports;

public:
    HttpBluetoothReporter(HttpClientAdapter *httpClientAdapter);
    void initScan();
    void scanDone();
    void bluBasicReport(BLEBasicReport *report);

};


char *safeCopy(const char *arg);

#endif

