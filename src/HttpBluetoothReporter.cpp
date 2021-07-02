
#include <Arduino.h>
#include "BluetoothReporter.h"

// #include <SPI.h>
#include <WiFi.h>
// #include <HttpClient.h>
// #include <Ethernet.h>
// #include <EthernetClient.h>

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
}

void HttpBluetoothReporter::initScan()
{
    Serial.println("zScan starting");
}

void HttpBluetoothReporter::scanDone()
{
    Serial.println("zScan done");
}

void connectToWifiNetwork() {
 Serial.println("Initializing HTTP Bluetooth Reporter");

     char *ssid     = "Telenor7329hos";
     char *password = "adscljsaupblg";

    delay(10);
    Serial.println('\n');

    WiFi.begin(ssid, password);
    Serial.print("Connecting to ");
    Serial.print(ssid);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print('.');
    }

    Serial.println('\n');
    Serial.println("Connection established!");
    Serial.print("IP address:\t");
    Serial.println(WiFi.localIP());
}

HttpBluetoothReporter::HttpBluetoothReporter()
{
   connectToWifiNetwork();
}
