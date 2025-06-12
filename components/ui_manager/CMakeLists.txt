idf_component_register(SRCS "ui_manager.c"
                    INCLUDE_DIRS "." "../ui_pages/home_page" "../ui_pages/wifi_bt_page" "../ui_pages/file_explorer_page" "../ui_pages/settings_page" "../ui_pages/console_debug_page"
                    REQUIRES "lvgl" "ui_pages")

