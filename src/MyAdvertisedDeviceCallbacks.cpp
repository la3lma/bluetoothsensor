#include <Arduino.h>

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#include <BLEEddystoneURL.h>
#include <BLEEddystoneTLM.h>
#include <BLEBeacon.h>

#include "MyAdvertisedDeviceCallbacks.h"

#define ENDIAN_CHANGE_U16(x) ((((x)&0xFF00) >> 8) + (((x)&0xFF) << 8))

/*
char *safeCopy(const char *arg)
{
    char *result = (char *)malloc(strlen(arg) + 1);
    strcpy(result, arg);
    return (result);
}
*/

void MyAdvertisedDeviceCallbacks::onResult(BLEAdvertisedDevice advertisedDevice)
{

    std::string adv = advertisedDevice.toString();
    Serial.println("Serialized representation of beacon");
    Serial.println(adv.c_str());

    // Build the basic report (raw data). We'll later augment with
    // specialized data interpretations if relevant (maybe, or perhaps we should just send
    // everything along to the backend?)
    BLEBasicReport *report = new BLEBasicReport;

    report->bleAddress = safeCopy(advertisedDevice.getAddress().toString().c_str()); 
    if (advertisedDevice.haveAppearance())
    {
        report->appearance = advertisedDevice.getAppearance();
        report->haveAppearance = true;
    }
    else
    {
        report->haveAppearance = false;
    }

    if (advertisedDevice.haveName())
    {
        report->name = advertisedDevice.getName().c_str();
        report->haveName = true;
    }
    else
    {
        report->haveName = false;
    }

    if (advertisedDevice.haveRSSI())
    {
        report->rssi = advertisedDevice.getRSSI();
        report->haveRSSI = true;
    }
    else
    {
        report->haveRSSI = false;
    }

    if (advertisedDevice.haveTXPower())
    {
        report->txPower = advertisedDevice.getTXPower();
        report->haveTXPower = true;
    }
    else
    {
        report->haveTXPower = false;
    }

    if (advertisedDevice.haveServiceData())
    {
        report->haveServiceData = true;
        // TODO: Get actual data
    }
    else
    {
        report->haveServiceData = false;
    }

    if (advertisedDevice.haveServiceUUID())
    {
        report->haveServiceUUID = true;
        BLEUUID devUUID = advertisedDevice.getServiceUUID();
        report->serviceUUID = safeCopy(devUUID.toString().c_str()); // TODO: Pick out real data
    }
    else
    {
        report->haveServiceUUID = false;
    }

    if (!report->haveServiceUUID)
    {
        if (advertisedDevice.haveManufacturerData() == true)
        {
            std::string strManufacturerData = advertisedDevice.getManufacturerData();

            uint8_t cManufacturerData[100];
            strManufacturerData.copy((char *)cManufacturerData, strManufacturerData.length(), 0);

            // TODO: What is this pattern matching, exactly?
            if (strManufacturerData.length() == 25 && cManufacturerData[0] == 0x4C && cManufacturerData[1] == 0x00)
            {
                BLEBeacon oBeacon = BLEBeacon();
                oBeacon.setData(strManufacturerData);

                // New
                IBeaconReport *ibr = new IBeaconReport();
                ibr->manufacturerId = oBeacon.getManufacturerId();
                ibr->major = ENDIAN_CHANGE_U16(oBeacon.getMajor());
                ibr->minor = ENDIAN_CHANGE_U16(oBeacon.getMinor());
                ibr->signalPower = oBeacon.getSignalPower();
                ibr->proximityUuid = safeCopy(oBeacon.getProximityUUID().toString().c_str());

                report->iBeaconReports.push_back(ibr);
            }
           // else
           // {
           //     // Old
           //    //  this->myReporter->reportOBeacon(strManufacturerData, cManufacturerData);
 //
            //     // New  (Put it in a string instead)  FIX LATER
             //    /*  Serial.println("Found another manufacturers beacon!");
              //   Serial.printf("strManufacturerData: %d ", strManufacturerData.length());
              //   int outputLength = 0;
              //   for (int i = 0; i < strManufacturerData.length(); i++)
              //   {
              //       Serial.printf("[%X]", cManufacturerData[i]);
             //    }
              //   Serial.println(); */
            // }
        }
    }

    this->myReporter->bluBasicReport(report);

    // This latter part seems to be looking at the payload and parsing that somehow
    // in particuluar, assuming some "eddistone" type of bluetooth  beacon device
    // I think we're just dropping that for now.
    /*

    uint8_t *payLoad = advertisedDevice.getPayload();

    BLEUUID checkUrlUUID = (uint16_t)0xfeaa;

    if (advertisedDevice.getServiceUUID().equals(checkUrlUUID))
    {
        if (payLoad[11] == 0x10)
        {
            Serial.println("Found an EddystoneURL beacon!");
            BLEEddystoneURL foundEddyURL = BLEEddystoneURL();
            std::string eddyContent((char *)&payLoad[11]); // incomplete EddystoneURL struct!

            foundEddyURL.setData(eddyContent);
            std::string bareURL = foundEddyURL.getURL();
            if (bareURL[0] == 0x00)
            {
                size_t payLoadLen = advertisedDevice.getPayloadLength();
                Serial.println("DATA-->");
                for (int idx = 0; idx < payLoadLen; idx++)
                {
                    Serial.printf("0x%08X ", payLoad[idx]);
                }
                Serial.println("\nInvalid Data");
                return;
            }

            Serial.printf("Found URL: %s\n", foundEddyURL.getURL().c_str());
            Serial.printf("Decoded URL: %s\n", foundEddyURL.getDecodedURL().c_str());
            Serial.printf("TX power %d\n", foundEddyURL.getPower());
            Serial.println("\n");
        }
        else if (payLoad[11] == 0x20)
        {
            Serial.println("Found an EddystoneTLM beacon!");
            BLEEddystoneTLM foundEddyURL = BLEEddystoneTLM();
            std::string eddyContent((char *)&payLoad[11]); // incomplete EddystoneURL struct!

            eddyContent = "01234567890123";

            for (int idx = 0; idx < 14; idx++)
            {
                eddyContent[idx] = payLoad[idx + 11];
            }

            foundEddyURL.setData(eddyContent);
            Serial.printf("Reported battery voltage: %dmV\n", foundEddyURL.getVolt());
            Serial.printf("Reported temperature from TLM class: %.2fC\n", (double)foundEddyURL.getTemp());
            int temp = (int)payLoad[16] + (int)(payLoad[15] << 8);
            float calcTemp = temp / 256.0f;
            Serial.printf("Reported temperature from data: %.2fC\n", calcTemp);
            Serial.printf("Reported advertise count: %d\n", foundEddyURL.getCount());
            Serial.printf("Reported time since last reboot: %ds\n", foundEddyURL.getTime());
            Serial.println("\n");
            Serial.print(foundEddyURL.toString().c_str());
            Serial.println("\n");
        }
    }
    */
}
