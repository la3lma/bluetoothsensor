
#include <Arduino.h>
#include <ArduinoJson.h>
#include <list>
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
    DeviceNameReport *dnr = new DeviceNameReport();
    dnr->deviceName = safeCopy(dname);
    this->deviceNameReports.push_back(dnr);
}

void HttpBluetoothReporter::reportServiceUUID(const char *uuid)
{
    ServiceUuidReport *dnr = new ServiceUuidReport();
    dnr->uuid = safeCopy(uuid);
    this->serviceUuidReports.push_back(dnr);
}

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

void HttpBluetoothReporter::reportIBeacon(int manufacturerId, int major, int minor, const char *proximityUUID, int signalPower)
{

    DynamicJsonDocument doc(2000);
    doc["manufacturerId"] = manufacturerId;
    doc["major"] = major;
    doc["minor"] = minor;
    doc["proximityUUID"] = proximityUUID;
    doc["power"] = signalPower;
    // serializeJson(doc, Serial);
    // this->things.push_back("Something");

    String json;
    serializeJson(doc, json);
    // this->things.push_back(json);
}

void HttpBluetoothReporter::initScan()
{
    Serial.println("zScan starting");

    this->deviceNameReports.clear();
}

void HttpBluetoothReporter::scanDone()
{

    // TODO:
    // * Build json doc
    // * Build string based on json doc.
    // * Free data used to build json doc.
    // * Send data over the wire.
    // * Free data just sent over the wire.
    Serial.println("zScan done<1>");

    // Build json document

    DynamicJsonDocument doc(20000);
    JsonArray deviceNames = doc.createNestedArray("deviceNames");

    std::list<DeviceNameReport *>::iterator dr_it;
    for (dr_it = this->deviceNameReports.begin(); dr_it != this->deviceNameReports.end(); dr_it++)
    {
        DeviceNameReport *drn_ptr = *dr_it;
        char *deviceName = drn_ptr->deviceName;
        deviceNames.add(deviceName);
    }

    // Print json doc.
    String json;
    serializeJson(doc, json);
    Serial.println("json doc is: ");
    Serial.println(json);

    // TBD: Send it over the wire
    this->httpClientAdapter->sendJsonString(json);

    // Free the device name reports and the strings they use.
    // XXX std::list<DeviceNameReport *>::iterator dr_it;
    for (dr_it = this->deviceNameReports.begin(); dr_it != this->deviceNameReports.end(); dr_it++)
    {
        DeviceNameReport *drn_ptr = *dr_it;
        char *deviceName = drn_ptr->deviceName;

        free(deviceName);
        free(drn_ptr);
    }
    Serial.println("zScan done<3>");
}

HttpBluetoothReporter::HttpBluetoothReporter(HttpClientAdapter *httpClientAdapter)
{
    this->httpClientAdapter = httpClientAdapter;
}
