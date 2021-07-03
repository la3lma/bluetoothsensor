
// #include <SPI.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include "HttpClientAdapter.h"

// https://github.com/arduino-libraries/ArduinoHttpClient/blob/master/examples/SimplePost/SimplePost.ino


// https://RandomNerdTutorials.com/esp32-http-get-post-arduino/
void HttpClientAdapter::sendHttpPost()
{
    if (WiFi.status() == WL_CONNECTED)
    {
        WiFiClient client;
        HTTPClient http;
         Serial.println("Preparing to send HTTP");

        // Your Domain name with URL path or IP address with path
        http.begin(client, this->serverName);

        // Specify content-type header
        http.addHeader("Content-Type", "application/x-www-form-urlencoded");
        // Data to send with HTTP POST
        String httpRequestData = "api_key=tPmAT5Ab3j7F9&sensor=BME280&value1=24.25&value2=49.54&value3=1005.14";
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


void HttpClientAdapter::initializeHttpClient() {
    // Maybe nuke this?
}

HttpClientAdapter::HttpClientAdapter(const char *serverName)
{
    this->serverName = serverName;
    this->connectToWifiNetwork();
    this->sendHttpPost();
}