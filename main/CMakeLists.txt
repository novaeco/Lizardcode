idf_component_register(
    SRCS "main.c"
    INCLUDE_DIRS "."
    REQUIRES "lvgl_port" "ui_manager" "lvgl_keyboard" "wifi_service" "bluetooth_service" "ota_service" "storage_service" "i18n_service" "lvgl_theme" "bt"
)

