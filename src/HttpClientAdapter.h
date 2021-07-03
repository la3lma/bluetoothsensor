#ifndef HttpClientAdapter_h
#define HttpClientAdapter_h



#include <Arduino.h>

class HttpClientAdapter
{
private:
    void connectToWifiNetwork();
    void sendHttpPost();
    void initializeHttpClient();
    const char *serverName;

public:
    HttpClientAdapter(const char *serverName);
};

#endif /* HttpClientAdapter_h */
