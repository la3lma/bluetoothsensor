
#include <Arduino.h>
#include <ArduinoJson.h>

#include "BluetoothReporter.h"

void HttpBluetoothReporter::reportDeviceName(const char *dname)
{
    Serial.print("zDevice name: ");
    Serial.println(dname);
    Serial.println("");
}

void HttpBluetoothReporter::reportServiceUUID(String uuid)
{
    Serial.print("Fzound ServiceUUID: ");
    Serial.println(uuid);
    Serial.println("");

    taticJsonDocument<2000> doc;
    doc["UUID"] = uuid;
    serializeJson(doc, Serial);
}

void HttpBluetoothReporter::reportOBeacon(std::string strManufacturerData, uint8_t *cManufacturerData)
{
    Serial.println("Found another manufacturers beacon!");
    Serial.printf("strManufacturerData: %d ", strManufacturerData.length());
    for (int i = 0; i < strManufacturerData.length(); i++)
    {
        Serial.printf("[%X]", cManufacturerData[i]);
    }
    Serial.printf("\n");
}

void HttpBluetoothReporter::reportIBeacon(int manufacturerId, int major, int minor, const char *proximityUUID, int signalPower)
{
    Serial.println("zFound an iBeacon!");

    Serial.printf("ziBeacon Frame\n");
    Serial.printf("ID: %04X Major: %d Minor: %d UUID: %s Power: %d\n",
                  manufacturerId,
                  major,
                  minor,
                  proximityUUID,
                  signalPower);

    StaticJsonDocument<2000> doc;
    doc["manufacturerId"] = manufacturerId;
    doc["major"] = major;
    doc["minor"] = minor;
    doc["proximityUUID"] = proximityUUID;
    doc["power"] = signalPower;
    serializeJson(doc, Serial);
}

void HttpBluetoothReporter::initScan()
{
    Serial.println("zScan starting");
    // this->doc = new DynamicJsonDocument(20000);
    // this->doc["banana"] = "fly"
}

void HttpBluetoothReporter::scanDone()
{
    Serial.println("zScan done");
    // serializeJson(this->doc, Serial);
    // free (this->doc);
    // this->doc = NULL;

    StaticJsonDocument<200> doc;
    doc["sensor"] = "gps";
    doc["time"] = 1351824120;
    serializeJson(doc, Serial);
}

HttpBluetoothReporter::HttpBluetoothReporter()
{
}
