
#include "WifiConnectivity.h"

static const char* TAG = "WifiConnectivity";

void connectToWifiNetwork()
{
    ESP_LOGV(TAG, "Initializing WIFI connection");

    // TODO: Find some better way to hide this information,
    //       hidden file, or something else.
    const char *ssid = "Telenor7329hos";
    const char *password = "adscljsaupblg";

    WiFi.begin(ssid, password);
    ESP_LOGV(TAG, "Connecting to %s", ssid);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        ESP_LOGV(TAG, ".");
    }

    ESP_LOGV(TAG, "Connected: SSID=%s, IP=%s, MAC=%s", 
        ssid, 
        WiFi.localIP().toString().c_str(), 
        WiFi.macAddress().c_str());
}