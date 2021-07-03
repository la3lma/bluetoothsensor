#include <Arduino.h>

class HttpClientAdapter
{
private:
    void connectToWifiNetwork();
    void sendHttpPost();
    void initializeHttpClient();

public:
    HttpClientAdapter();
};
