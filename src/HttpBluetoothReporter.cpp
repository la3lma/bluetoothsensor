
#include <Arduino.h>
#include <ArduinoJson.h>
#include <list>
#include <map>
#include <iterator>

#include "BluetoothReporter.h"

// TODO:    For each of the incoming method calls, generate full JSON, and put it in a
//          string (or a list of strings, whatever is the simplest).  That list of strings (or whatever),
//          is then printed as an array of json objects, and that's how we serialize this stuff.
//          C++ is such a shitty language.

char *safeCopy(const char *arg)
{
    char *result = (char *)malloc(strlen(arg) + 1);
    strcpy(result, arg);
    return (result);
}

void HttpBluetoothReporter::reportDeviceName(const char *dname)
{
    std::string key(dname);
    if (this->deviceNameReports.find(key) == this->deviceNameReports.end())
    {
        DeviceNameReport *value = new DeviceNameReport();
        value->deviceName = safeCopy(dname);
        this->deviceNameReports.insert({key, value});
    }
}

void HttpBluetoothReporter::reportServiceUUID(const char *uuid)
{
    std::string key(uuid);
    if (this->serviceUuidReports.find(key) == this->serviceUuidReports.end())
    {
        ServiceUuidReport *value = new ServiceUuidReport();
        value->uuid = safeCopy(uuid);
        this->serviceUuidReports.insert({key, value});
    }
}

// This is in fact the majority of the data we see.  The reporting form is a bit useless, so
// we shouldn't use it, instead we should rewrite so that the relevant data is actually extracted
// in a format that makes sense when we send it  over http to the home office.
void HttpBluetoothReporter::reportOBeacon(std::string strManufacturerData, uint8_t *cManufacturerData)
{
    Serial.println("Found another manufacturers beacon!");
    /* Serial.printf("strManufacturerData: %d ", strManufacturerData.length());
    for (int i = 0; i < strManufacturerData.length(); i++)
    {
        Serial.printf("[%X]", cManufacturerData[i]);
    }
    Serial.printf("\n"); */

    // This doesn't work very well. Simply reports a single "L"
    StaticJsonDocument<2000> doc;
    doc["manufacturerData"] = strManufacturerData;
    String json;
    serializeJson(doc, json);
    // this->things.push_back(json);
}

void HttpBluetoothReporter::reportIBeacon(int manufacturerId, int major, int minor, const char *proximityUuid, int signalPower)
{
    IBeaconReport *ibr = new IBeaconReport();
    ibr->manufacturerId = manufacturerId;
    ibr->major = major;
    ibr->minor = minor;
    ibr->signalPower = signalPower;
    ibr->proximityUuid = safeCopy(proximityUuid);

    this->iBeaconReports.push_back(ibr);
}

void HttpBluetoothReporter::initScan()
{
    Serial.println("zScan starting");

    //
    // Free the device name reports and the strings they use.
    //
    for (auto it = this->deviceNameReports.begin(); it != this->deviceNameReports.end(); it++)
    {
        DeviceNameReport *ptr = it->second;
        char *deviceName = ptr->deviceName;
        free(deviceName);
        free(ptr);
    }

    for (auto it = this->serviceUuidReports.begin(); it != this->serviceUuidReports.end(); it++)
    {
        ServiceUuidReport *ptr = it->second;
        char *uuid = ptr->uuid;
        free(uuid);
        free(ptr);
    }

    for (auto it = this->iBeaconReports.begin(); it != this->iBeaconReports.end(); it++)
    {
        IBeaconReport *ptr = *it;
        char *uuid = ptr->proximityUuid;
        free(uuid);
        free(ptr);
    }

    // TODO: Figure out if we can do the deletions we do above, using the
    //       destructor method somehow?
    this->deviceNameReports.clear();
    this->serviceUuidReports.clear();
    this->iBeaconReports.clear();
}

void HttpBluetoothReporter::scanDone()
{

    // TODO:
    // * Build json doc
    // * Build string based on json doc.
    // * Free data used to build json doc.
    // * Send data over the wire.
    // * Free data just sent over the wire.
    // * Rewrite data collection to only send one report per identified instance, per reporting cycle.
    //    ... using this https://en.cppreference.com/w/cpp/container/map
    Serial.println("zScan done<1>");

    // Build json document

    DynamicJsonDocument doc(20000);
    JsonArray deviceNames = doc.createNestedArray("deviceNames");

    for (auto dr_it = this->deviceNameReports.begin(); dr_it != this->deviceNameReports.end(); dr_it++)
    {
        DeviceNameReport *drn_ptr = dr_it->second;
        char *deviceName = drn_ptr->deviceName;
        deviceNames.add(deviceName);
    }

    JsonArray uuids = doc.createNestedArray("uuids");
    for (auto uuid_it = this->serviceUuidReports.begin(); uuid_it != this->serviceUuidReports.end(); uuid_it++)
    {
        ServiceUuidReport *ptr = uuid_it->second;
        char *uuid = ptr->uuid;
        uuids.add(uuid);
    }

    JsonObject iBeacons = doc.createNestedObject("iBeacons");
    for (auto ibeacon_it = this->iBeaconReports.begin(); ibeacon_it != this->iBeaconReports.end(); ibeacon_it++)
    {
        IBeaconReport *ptr = *ibeacon_it;
        char *proximityUuid = safeCopy(ptr->proximityUuid);

        iBeacons["manufacturerId"] = ptr->manufacturerId;
        iBeacons["major"] = ptr->major;
        iBeacons["minor"] = ptr->minor;
        iBeacons["proximityUUID"] = proximityUuid;
        iBeacons["power"] = ptr->signalPower;
    }

    // Print json doc.
    String json;
    serializeJsonPretty(doc, json);
    Serial.println("json doc is: ");
    Serial.println(json);

    // Send it over the wire
    this->httpClientAdapter->sendJsonString(json);

    Serial.println("zScan done<3>");
}

HttpBluetoothReporter::HttpBluetoothReporter(HttpClientAdapter *httpClientAdapter)
{
    this->httpClientAdapter = httpClientAdapter;
}
