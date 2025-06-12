idf_component_register(SRCS "ota_service.c"
                    INCLUDE_DIRS "."
                    REQUIRES "esp_http_client" "esp_https_ota" "esp_ota_ops" "nvs_flash")

