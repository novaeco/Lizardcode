#include "ota_service.h"
#include "esp_log.h"
#include "esp_ota_ops.h"
#include "esp_http_client.h"
#include "esp_https_ota.h"

static const char *TAG = "OTA_SERVICE";

esp_err_t ota_service_init(void)
{
    ESP_LOGI(TAG, "OTA service initialized.");
    return ESP_OK;
}

esp_err_t ota_service_start_update(const char *url)
{
    ESP_LOGI(TAG, "Starting OTA update from %s", url);

    esp_http_client_config_t config = {
        .url = url,
        .cert_pem = NULL, // No certificate for HTTP, provide if using HTTPS with custom certs
        .timeout_ms = 5000,
    };

    esp_err_t ret = esp_https_ota(&config);
    if (ret == ESP_OK) {
        ESP_LOGI(TAG, "OTA update successful, restarting...");
        esp_restart();
    } else {
        ESP_LOGE(TAG, "OTA update failed: %s", esp_err_to_name(ret));
    }
    return ret;
}


