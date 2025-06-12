idf_component_register(SRCS "storage_service.c"
                    INCLUDE_DIRS "."
                    REQUIRES "spiffs" "fatfs" "sdmmc")

