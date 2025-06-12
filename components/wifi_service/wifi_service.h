#ifndef WIFI_SERVICE_H
#define WIFI_SERVICE_H

#include "esp_err.h"
#include "esp_wifi_types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*wifi_scan_cb_t)(uint16_t ap_count, wifi_ap_record_t *ap_records);
typedef void (*wifi_connect_cb_t)(esp_err_t result);

/**
 * @brief Initialize the Wi-Fi service.
 * @return ESP_OK on success, otherwise an error code.
 */
esp_err_t wifi_service_init(void);

/**
 * @brief Start Wi-Fi scan.
 * @param cb Callback function to be called after scan completes.
 * @return ESP_OK on success, otherwise an error code.
 */
esp_err_t wifi_service_scan_start(wifi_scan_cb_t cb);

/**
 * @brief Connect to a Wi-Fi access point.
 * @param ssid SSID of the access point.
 * @param password Password of the access point.
 * @param cb Callback function to be called after connection attempt.
 * @return ESP_OK on success, otherwise an error code.
 */
esp_err_t wifi_service_connect(const char *ssid, const char *password, wifi_connect_cb_t cb);

/**
 * @brief Disconnect from Wi-Fi.
 * @return ESP_OK on success, otherwise an error code.
 */
esp_err_t wifi_service_disconnect(void);

#ifdef __cplusplus
}
#endif

#endif // WIFI_SERVICE_H


