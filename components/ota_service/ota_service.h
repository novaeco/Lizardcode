#ifndef OTA_SERVICE_H
#define OTA_SERVICE_H

#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initialize the OTA service.
 * @return ESP_OK on success, otherwise an error code.
 */
esp_err_t ota_service_init(void);

/**
 * @brief Start OTA update.
 * @param url URL of the firmware image.
 * @return ESP_OK on success, otherwise an error code.
 */
esp_err_t ota_service_start_update(const char *url);

#ifdef __cplusplus
}
#endif

#endif // OTA_SERVICE_H


