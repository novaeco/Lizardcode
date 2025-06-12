idf_component_register(SRCS "bluetooth_service.c"
                    INCLUDE_DIRS "."
                    REQUIRES "esp_bt" "esp_gap_ble" "esp_gatts_api" "esp_bt_main" "bt" "nimble")

