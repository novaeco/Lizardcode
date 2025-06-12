idf_component_register(SRCS "home_page/home_page.c"
                     "wifi_bt_page/wifi_bt_page.c"
                     "file_explorer_page/file_explorer_page.c"
                     "settings_page/settings_page.c"
                     "console_debug_page/console_debug_page.c"
                     INCLUDE_DIRS "home_page" "wifi_bt_page" "file_explorer_page"
                                  "settings_page" "console_debug_page"
                     REQUIRES lvgl storage_service i18n_service)
