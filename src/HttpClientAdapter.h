#include <Arduino.h>

class HttpClientAdapter
{
public:
    HttpClientAdapter();
    void connectToWifiNetwork();
    void sendHttpPost();

};
