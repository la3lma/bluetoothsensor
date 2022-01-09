#ifndef HttpClientAdapter_h
#define HttpClientAdapter_h

#include <Arduino.h>

class HttpClientAdapter
{
private:
    void sendHttpSamplePacket();
    void initializeHttpClient();
    const char *serverName;

public:
    HttpClientAdapter(const char *serverName);
    int sendJsonString(const String &report);
};

#endif /* HttpClientAdapter_h */
