#include <ArduinoJson.h>
#include <WiFi.h>
#include <list>
#include <map>
#include <iterator>
#include <string>
#include <esp_task_wdt.h>

#include "JsonProducingBluetoothReporter.h"

static const char* TAG = "JsonProducingBluetoothRepoter";

// TODO:    For each of the incoming method calls, generate full JSON, and put it in a
//          string (or a list of strings, whatever is the simplest).  That list of strings,
//          is then printed as an array of json objects, and that's how we serialize this stuff.

char *safeCopy(const char *arg)
{
    char *result = (char *)malloc(strlen(arg) + 1);
    strcpy(result, arg);
    return (result);
}


char *safeStringCopy(std::string arg)
{
    char *cstr = (char *)arg.c_str();
    char *result = (char *)malloc(strlen(cstr) + 1);
    strcpy(result, cstr);
    return (result);
}

void JsonProducingBluetoothReporter::initScan()
{
    Serial.println("zScan starting");

    this->reports.clear();
}

void BLEBasicReport::toJson(JsonObject json)
{

    json["bleAddress"] = this->bleAddress;

    if (this->haveName)
    {
        Serial.print("Json-recording name\n");
        json["name"] = this->name;
    }

    if (this->haveRSSI)
    {
        json["rssi"] = this->rssi;
    }

    if (this->haveServiceUUID)
    {
        json["serviceUUID"] = this->serviceUUID;
        json["16bitUUID"] = this->serviceUUIDVendorCode;
    }

    if (this->haveTXPower)
    {
        json["haveTXPower"] = this->txPower;
    }

    // TODO : Missing servicedata

    if (!this->iBeaconReports.empty())
    {
        JsonArray iBeaconReports = json.createNestedArray("iBeaconReports");
        for (auto it = this->iBeaconReports.begin(); it != this->iBeaconReports.end(); it++)
        {
            IBeaconReport *ptr = *it;
            JsonObject iBeacon = iBeaconReports.createNestedObject();
            iBeacon["manufacturerId"] = ptr->manufacturerId;
            iBeacon["major"] = ptr->major;
            iBeacon["minor"] = ptr->minor;
            iBeacon["proximityUUID"] = ptr->proximityUuid;
            iBeacon["power"] = ptr->signalPower;
        }
    }
}

bool JsonProducingBluetoothReporter::hasKey(std::string &key)
{
    return (this->reports.find(key) != this->reports.end());
}

BLEBasicReport * JsonProducingBluetoothReporter::registerNewReport(std::string bleAddress)
{
    esp_task_wdt_reset(); // Reset watchdog timer to avoid timeout.
    BLEBasicReport *report = new BLEBasicReport();
    report->bleAddress = bleAddress;

    this->reports.insert({bleAddress, report});
    return report;
}

String JsonProducingBluetoothReporter::scanDone()
{
    ESP_LOGV(TAG, "Starting to produce bluetooth json doc");
    DynamicJsonDocument doc(25000);  // TODO: Make this number as large as possible.
    ESP_LOGV(TAG, "Allocated dynamic result buffer");
    doc.clear();

    String wifiMac = WiFi.macAddress();
    doc["scannerID"]["wifiMAC"] = wifiMac.c_str();

    JsonArray reports = doc.createNestedArray("bleReports");

    for (auto it = this->reports.begin(); it != this->reports.end(); it++)
    {
        ESP_LOGV(TAG, "Looping over elements");
        const std::string key = it->first;
        BLEBasicReport ptr = *(it->second);
        JsonObject nested = reports.createNestedObject();
        ptr.toJson(nested);
    }
    
    ESP_LOGV(TAG, "Serializing result");

    // Print json doc.
    String output;
    serializeJson(doc, output);

    ESP_LOGV(TAG, "Returning result");
    return output;
}

JsonProducingBluetoothReporter::JsonProducingBluetoothReporter()
{
}
