#ifndef BLUETOOTH_SERVICE_H
#define BLUETOOTH_SERVICE_H

#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initialize the Bluetooth service (BLE).
 * @return ESP_OK on success, otherwise an error code.
 */
esp_err_t bluetooth_service_init(void);

/**
 * @brief Start BLE advertising.
 * @return ESP_OK on success, otherwise an error code.
 */
esp_err_t bluetooth_service_start_advertising(void);

/**
 * @brief Stop BLE advertising.
 * @return ESP_OK on success, otherwise an error code.
 */
esp_err_t bluetooth_service_stop_advertising(void);

#ifdef __cplusplus
}
#endif

#endif // BLUETOOTH_SERVICE_H


