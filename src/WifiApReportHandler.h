#ifndef WifiApReportHandler_h
#define WifiApReportHandler_h

#include <esp_http_server.h>

esp_err_t get_wifi_ap_report_handler(httpd_req_t *req);

#endif