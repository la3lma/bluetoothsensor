
#include <WiFi.h>
#include <HTTPClient.h>
#include "HttpClientAdapter.h"

// https://github.com/arduino-libraries/ArduinoHttpClient/blob/master/examples/SimplePost/SimplePost.ino

// https://RandomNerdTutorials.com/esp32-http-get-post-arduino/
// This method is intended to be used only for debugging, when thing sowk, drop it.
void HttpClientAdapter::sendHttpSamplePacket()
{
    if (WiFi.status() == WL_CONNECTED)
    {
        WiFiClient client;
        HTTPClient http;
        Serial.println("Preparing to send HTTP");

        // Your Domain name with URL path or IP address with path
        http.begin(client, this->serverName);

        // Specify content-type header
        // http.addHeader("Content-Type", "application/x-www-form-urlencoded");
        // Data to send with HTTP POST
        // String httpRequestData = "api_key=tPmAT5Ab3j7F9&sensor=BME280&value1=24.25&value2=49.54&value3=1005.14";
        // Send HTTP POST request
        Serial.println("... posting");

        // int httpResponseCode = http.POST(httpRequestData);

        // If you need an HTTP request with a content type: application/json, use the following:
        http.addHeader("Content-Type", "application/json");
        int httpResponseCode = http.POST("{\"api_key\":\"tPmAT5Ab3j7F9\",\"sensor\":\"BME280\",\"value1\":\"24.25\",\"value2\":\"49.54\",\"value3\":\"1005.14\"}");

        // If you need an HTTP request with a content type: text/plain
        //http.addHeader("Content-Type", "text/plain");
        //int httpResponseCode = http.POST("Hello, World!");

        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);

        // Free resources
        http.end();
    }
    else
    {
        Serial.println("WiFi Disconnected");
    }
}

void HttpClientAdapter::connectToWifiNetwork()
{
    Serial.println("Initializing HTTP Bluetooth Reporter");

    const char *ssid = "Telenor7329hos";
    const char *password = "adscljsaupblg";

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

void HttpClientAdapter::initializeHttpClient()
{
    // Maybe nuke this?
}

// TODO: Handle error situations (on-net/off net etc.)
int HttpClientAdapter::sendJsonString(const String &jsonString)
{
    if (WiFi.status() == WL_CONNECTED)
    {
        WiFiClient client;
        HTTPClient http;
        Serial.println("Preparing to zend HTTP");
        Serial.println("Size of http json doc is: ");
        Serial.println(jsonString.length());

        // Your Domain name with URL path or IP address with path
        http.begin(client, this->serverName);

        http.addHeader("Content-Type", "application/json");

        int httpResponseCode = http.POST(jsonString);
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);

        // Free resources
        http.end();

        if (httpResponseCode < 0)
        {
            return httpResponseCode;
        }
        else
        {
            return jsonString.length();
        }
    }
    else
    {
        Serial.println("WiFi Disconnected");
        return 0;
    }
}

HttpClientAdapter::HttpClientAdapter(const char *serverName)
{
    this->serverName = serverName;
    this->connectToWifiNetwork();
    this->sendHttpSamplePacket();
    this->sendJsonString("{\"clientType\":\"experimental\"}");
    String str("{\"clientType\":\"evenMoreExperimental\"}");
    this->sendJsonString(str);

/* DELETE THIS
    // Now try to find the MTU for the packet sender
    // Use binary search.

    int min = 1;
    int max = 20000;

    while ((min + 1) < max)
    {
        int pivot = min + ((max - min) / 2);

        Serial.print("Min = ");
        Serial.println(min);
        Serial.print("Max = ");
        Serial.println(max);
        Serial.print("pivot = ");
        Serial.println(pivot);

        String s = "";
        for (int i = 0 ; i < pivot ; i++){
            s = s + "*";
        }
        
        int slength = s.length();
        Serial.print("slength = ");
        Serial.println(slength);

        int sentLength = this->sendJsonString(s);

        if (sentLength < 1)
        {
            continue;
        }

        if (sentLength == pivot)
        {
            min = pivot;
        }
        else
        {
            max = pivot;
        }
    }

    Serial.println("===->");
    Serial.print("Min = ");
    Serial.println(min);
    Serial.print("Max = ");
    Serial.println(max);
    */
}