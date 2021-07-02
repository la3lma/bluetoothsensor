#include <Arduino.h>
#include "BluetoothReporter.h"

void SerialBluetoothReporter::reportDeviceName(const char *dname)
{
    Serial.print("Device name: ");
    Serial.println(dname);
    Serial.println("");
}

void SerialBluetoothReporter::reportServiceUUID(String uuid)
{
    Serial.print("Found ServiceUUID: ");
    Serial.println(uuid);
    Serial.println("");
}

void SerialBluetoothReporter::reportOBeacon(std::string strManufacturerData, uint8_t *cManufacturerData)
{
    Serial.println("Found another manufacturers beacon!");
    Serial.printf("strManufacturerData: %d ", strManufacturerData.length());
    for (int i = 0; i < strManufacturerData.length(); i++)
    {
        Serial.printf("[%X]", cManufacturerData[i]);
    }
    Serial.printf("\n");
}

void SerialBluetoothReporter::reportIBeacon(int manufacturerId, int major, int minor, const char *proximityUUID, int signalPower)
{
    Serial.println("Found an iBeacon!");

    Serial.printf("iBeacon Frame\n");
    Serial.printf("ID: %04X Major: %d Minor: %d UUID: %s Power: %d\n",
                  manufacturerId,
                  major,
                  minor,
                  proximityUUID,
                  signalPower);
}

void SerialBluetoothReporter::initScan()
{
    Serial.println("Scan starting");
}


void SerialBluetoothReporter::scanDone()
{
    Serial.println("Scan done");
}

