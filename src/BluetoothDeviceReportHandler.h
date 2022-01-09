#ifndef BluetoothDeviceReportHandler_h
#define BluetoothDeviceReportHandler_h

#include <esp_http_server.h>

esp_err_t get_bluetooth_device_report_handler(httpd_req_t *req);

#endif