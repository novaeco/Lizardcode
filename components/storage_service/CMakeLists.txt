idf_component_register(SRCS "storage_service.c"
                    INCLUDE_DIRS "."
                    REQUIRES "esp_spiffs" "fatfs" "sdmmc")

