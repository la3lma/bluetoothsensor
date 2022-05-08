#include <esp_http_server.h>
#include "esp_log.h"
#include "esp_wifi.h"
#include <ArduinoJson.h>


#include "WifiApReportHandler.h"

static const char* TAG = "WifiApReportHandler";
String scanAndReportWifiNetworks()
{

  ESP_LOGV(TAG, "Starting WIFI AP scan");

  DynamicJsonDocument doc(20000);

  // A json object to identify this particular scanner

  String wifiMac = WiFi.macAddress();
  doc["scannerID"]["wifiMAC"] = wifiMac;

  JsonArray reports = doc.createNestedArray("wifiApReports");

  // WiFi.scanNetworks will return the number of networks found
  int n = WiFi.scanNetworks();
  ESP_LOGV(TAG, "WIFI AP scan done, found %d networks.", n);
  if (n != 0)
  {
    for (int i = 0; i < n; ++i)
    {
      JsonObject nested = reports.createNestedObject();

      nested["SSID"] = WiFi.SSID(i);
      nested["RSSI"] = WiFi.RSSI(i);

      int encryptionType = WiFi.encryptionType(i);
      switch (encryptionType)
      {
      case WIFI_AUTH_OPEN:
        nested["encryption"] = "OPEN";
        break;

      case WIFI_AUTH_WEP:
        nested["encryption"] = "OPEN";
        break;
      case WIFI_AUTH_WPA_PSK:
        nested["encryption"] = "WPA_PSK";
        break;
      case WIFI_AUTH_WPA2_PSK:
        nested["encryption"] = "WPA2_PSK";
        break;
      case WIFI_AUTH_WPA_WPA2_PSK:
        nested["encryption"] = "WPA_WPA2_PSK";
        break;
      case WIFI_AUTH_WPA2_ENTERPRISE:
        nested["encryption"] = "WPA2_ENTERPRISE";
        break;
      case WIFI_AUTH_MAX:
        nested["encryption"] = "MAX";
        break;
      default:
        nested["encryption"] = "UNKNOWN";
        break;
      }
    }
  }


  // Print json doc.
  String json;
  serializeJsonPretty(doc, json);
  return json;
}


esp_err_t get_wifi_ap_report_handler(httpd_req_t *req)
{
    ESP_LOGV(TAG, "http get handler triggered");
    httpd_resp_set_type(req, "application/json;charset=utf-8");
    httpd_resp_send(req, scanAndReportWifiNetworks().c_str(), HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

