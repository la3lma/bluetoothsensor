#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#include <BLEEddystoneURL.h>
#include <BLEEddystoneTLM.h>
#include <BLEBeacon.h>
#include <esp_task_wdt.h>
#include "esp_log.h"

#include "MyAdvertisedDeviceCallbacks.h"

#define ENDIAN_CHANGE_U16(x) ((((x)&0xFF00) >> 8) + (((x)&0xFF) << 8))

static const char* TAG = "MyAdvertisedDeviceCallbacks";

void MyAdvertisedDeviceCallbacks::onResult(BLEAdvertisedDevice advertisedDevice)
{
    esp_task_wdt_reset();

    ESP_LOGV(TAG, "ESP free heap is %u",  ESP.getFreeHeap());

    // std::string adv = advertisedDevice.toString();

    // Get the ble address of the device, and use that to
    // remove duplicate entries for the same device.
    // This is a heuristic that will make us miss some
    // data but all of that data will be from the same device
    // within the same sampling period, so we can live with that.
    std::string bleAddress = advertisedDevice.getAddress().toString();

    if (this->myReporter->hasKey(bleAddress))
    {
         ESP_LOGV(TAG, "Duplicate detected: %s",bleAddress.c_str());
        return;
    }

    ESP_LOGV(TAG, "Unique beacon detected: %s", bleAddress.c_str());

    // Build the basic report (raw data). We'll later augment with
    // specialized data interpretations if relevant (maybe, or perhaps we should just send
    // everything along to the backend?)
    BLEBasicReport *rr = this->myReporter->registerNewReport(bleAddress.c_str());
    

    if (advertisedDevice.haveAppearance())
    {
        rr->appearance = advertisedDevice.getAppearance();
        rr->haveAppearance = true;
    }
    else
    {
        rr->haveAppearance = false;
    }

    if (advertisedDevice.haveName())
    {
        rr->name = safeStringCopy(advertisedDevice.getName());
        rr->haveName = true;
    }
    else
    {
        rr->haveName = false;
    }

    if (advertisedDevice.haveRSSI())
    {
        ESP_LOGV(TAG, "Reporting rssi");

        rr->rssi = advertisedDevice.getRSSI();
        rr->haveRSSI = true;
    }
    else
    {
        rr->haveRSSI = false;
    }

    if (advertisedDevice.haveTXPower())
    {
        ESP_LOGV(TAG, "Reporting txpower");
        rr->txPower = advertisedDevice.getTXPower();
        rr->haveTXPower = true;
    }
    else
    {
        rr->haveTXPower = false;
    }

    if (advertisedDevice.haveServiceData())
    {
        rr->haveServiceData = true;
        // TODO: Get actual data
    }
    else
    {
        rr->haveServiceData = false;
    }

    if (advertisedDevice.haveServiceUUID())
    {
        ESP_LOGV(TAG, "Reporting UUID");
        rr->haveServiceUUID = true;
        BLEUUID devUUID = advertisedDevice.getServiceUUID();
        rr->serviceUUID = devUUID.toString().c_str(); // ???    safeStringCopy(devUUID.toString());
        // TODO: Pick out vendor code and serve separately.

        rr->serviceUUIDVendorCode = rr->serviceUUID.substr(4,4);
    }
    else
    {
        rr->haveServiceUUID = false;
    }

    if (!rr->haveServiceUUID)
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
                ESP_LOGV(TAG, "Reporting ibeacon");
                IBeaconReport *ibr = new IBeaconReport();
                ibr->manufacturerId = oBeacon.getManufacturerId();
                ibr->major = ENDIAN_CHANGE_U16(oBeacon.getMajor());
                ibr->minor = ENDIAN_CHANGE_U16(oBeacon.getMinor());
                ibr->signalPower = oBeacon.getSignalPower();
                ibr->proximityUuid = oBeacon.getProximityUUID().toString().c_str();

                rr->iBeaconReports.push_back(ibr);
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

        ESP_LOGV(TAG, "Done registring beacon %s", bleAddress.c_str());
    }

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
