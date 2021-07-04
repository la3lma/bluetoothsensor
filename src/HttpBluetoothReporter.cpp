
#include <Arduino.h>
#include <ArduinoJson.h>
#include <list>
#include <iterator>

#include "BluetoothReporter.h"


// TODO:    For each of the incoming method calls, generate full JSON, and put it in a
//          string (or a list of strings, whatever is the simplest).  That list of strings (or whatever),
//          is then printed as an array of json objects, and that's how we serialize this stuff.
//          C++ is such a shitty language.

void HttpBluetoothReporter::reportDeviceName(const char *dname)
{
    StaticJsonDocument<2000> doc;
    doc["deviceName"] = dname;
    serializeJson(doc, Serial);
}

void HttpBluetoothReporter::reportServiceUUID(String uuid)
{
    StaticJsonDocument<2000> doc;
    doc["UUID"] = uuid;
    serializeJson(doc, Serial);
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
    this->things.push_back(json);
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
    this->things.push_back(json);
}

void HttpBluetoothReporter::initScan()
{
    Serial.println("zScan starting");
    // DynamicJsonDocument<20000> doc;
    // this->doc = &doc;
    // this->doc = new DynamicJsonDocument(20000);
    // this->doc["banana"] = "fly"

    // DynamicJsonDocument doc(20000);
    // this->doc = new(20000);

    // DynamicJsonDocument ddoc = *(this->doc);
    // (this->doc)["foo"] = "bar";

    this->things.clear();
}

void HttpBluetoothReporter::scanDone()
{

    Serial.println("zScan done<1>");
    std::list<String>::iterator it;
    for (it = this->things.begin(); it != this->things.end(); it++)
    {
        Serial.println(*it);
        // delete it;
    }
    Serial.println("zScan done<2>");
}

HttpBluetoothReporter::HttpBluetoothReporter(HttpClientAdapter *httpClientAdapter)
{
    this->httpClientAdapter = httpClientAdapter;
}
