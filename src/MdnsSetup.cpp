#include <mdns.h>
#include <esp32-hal-log.h>

#include "MdnsSetup.h"

void mdns_setup() {

    const int hostname_buffer_length = 40;
    static const char* TAG = "mdns_setup";

    ESP_LOGV(TAG, "Setting up MDNS");

    uint8_t chipid[6];
    esp_read_mac(chipid, (esp_mac_type_t) 0);
    char hostname_buffer[hostname_buffer_length];

    int err = snprintf(hostname_buffer, hostname_buffer_length, 
        "btmonitor-%02x%02x%02x%02x%02x%02x",
        chipid[0],chipid[1],chipid[2],chipid[3],chipid[4],chipid[5]
    );

    if (err == 0) {
        ESP_LOGI(TAG, "unable to set hostname, err = %d", err);
        return;
    }

    ESP_ERROR_CHECK(mdns_init());
    ESP_ERROR_CHECK(mdns_hostname_set(hostname_buffer));
    ESP_ERROR_CHECK(mdns_service_add(NULL, "_http", "_tcp", 80, NULL, 0));
    ESP_LOGI(TAG, "mdns hostname is %s", hostname_buffer);
}