
#include <Arduino.h>
#include <ArduinoJson.h>
#include <list>
#include <map>
#include <iterator>
#include <string>

#include "BluetoothReporter.h"
#include "BtleUuidDecoder.h"

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


char *safeStringCopy(std::string arg)
{
    char *cstr = (char *)arg.c_str();
    char *result = (char *)malloc(strlen(cstr) + 1);
    strcpy(result, cstr);
    return (result);
}

void HttpBluetoothReporter::initScan(String wifiMAC)
{
    Serial.println("zScan starting");
    this->wifiMAC = wifiMAC.c_str();
    this->reports.clear();
}

void BLEBasicReport::toJson(JsonObject json)
{

    json["bleAddress"] = this->bleAddress;

    if (this->haveName)
    {
        Serial.print("Json-printing name");
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

        int uuidCode = strtol(this->serviceUUIDVendorCode.c_str(), 0, 16);

        BtleUuidEntry *entry =   findUuidEntry(uuidCode);
        if (entry != NULL) {
            json["16bitUUIDAllocationType"] = entry->allocationType.c_str();
            json["16bitUUIDAllocatedFor"]  =  entry->allocatedFor.c_str();
        }
        
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

bool HttpBluetoothReporter::hasKey(std::string &key)
{
    return (this->reports.find(key) != this->reports.end());
}

BLEBasicReport * HttpBluetoothReporter::registerNewReport(std::string bleAddress)
{

    BLEBasicReport *report = new BLEBasicReport();
    report->bleAddress = bleAddress;

    this->reports.insert({bleAddress, report});
    return report;
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
    Serial.println("Scan done<1>");

    // There is a bug in the wifi/http clients that stops long reports from being written.
    // consequently we work around that by splitting the reports into multiple reports.
    // The stride parmeter determines how many bluetooth devices to include in each report,
    // it is a heuristically determined number (a.k.a. "wild guess").
    // The best solution would be to
    // get rid of whatever silly reason is stopping the probram from writing long
    // reports, but until we fix that, thits will at least send some reports

    const int stride = 5;
    for (int i = 1; i < this->reports.size(); i += stride)
    {

        // Build json document

        DynamicJsonDocument doc(20000);

        // A json object to identify this particular scanner
        doc["scannerID"]["wifiMAC"] = this->wifiMAC;

        // TBD:
        //   ... add a "wifiReports" element that reports on the
        //   wifi connections we see at this point (some magic may be
        //   needed to avoid the overrun issue handled by the nested for-loops)
        //   we see here.


        JsonArray reports = doc.createNestedArray("bleReports");

        int j = i;
        int k = stride;
        for (auto it = this->reports.begin(); it != this->reports.end(); it++)
        {
            if (j > 0 && --j > 0)
            {
                continue;
            }

            const std::string key = it->first;
            Serial.print("====---->");
            Serial.println(key.c_str());

            BLEBasicReport ptr = *(it->second);
            JsonObject nested = reports.createNestedObject();
            ptr.toJson(nested);

            if (--k < 0)
            {
                break;
            }
        }

        // Print json doc.
        String json;
        serializeJsonPretty(doc, json);
        Serial.println("json doc is: ");
        Serial.println(json);
        Serial.println("Size of json doc doc is: ");
        Serial.println(json.length());

        // Send it over the wire
        this->httpClientAdapter->sendJsonString(json);
    }

    Serial.println("Scan done<3>");
}

HttpBluetoothReporter::HttpBluetoothReporter(HttpClientAdapter *httpClientAdapter)
{
    this->httpClientAdapter = httpClientAdapter;
}
