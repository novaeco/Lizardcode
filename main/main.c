#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "lvgl_port.h"
#include "ui_manager.h"
#include "lvgl_keyboard.h"
#include "wifi_service.h"
#include "bluetooth_service.h"
#include "ota_service.h"
#include "storage_service.h"
#include "i18n_service.h"
#include "lvgl_theme.h"

static lv_obj_t *main_keyboard;

void app_main(void)
{
    ESP_LOGI("MAIN", "Application started!");

    // Initialize NVS (required for Wi-Fi and OTA)
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    // Initialize services
    wifi_service_init();
    bluetooth_service_init();
    ota_service_init();
    storage_service_init();
    i18n_service_init();

    // Set default language
    i18n_service_set_language(LANG_FR);

    // Initialize LVGL and its display/input drivers
    lvgl_port_init();

    // Initialize and set the custom LVGL theme
    lvgl_theme_init();

    // Create a FreeRTOS task for LVGL
    xTaskCreatePinnedToCore(lvgl_port_task, "lvgl_task", 4096 * 2, NULL, 1, NULL, 1);

    // Initialize UI Manager and create all pages
    lv_obj_t *base_screen = ui_manager_init();

    // Create the global virtual keyboard
    main_keyboard = lvgl_keyboard_create(base_screen);

    // Example: Create a text area on the home page to demonstrate keyboard usage
    // This part needs to be handled within the home_page.c or a dedicated UI setup function
    // For now, let\'s just show the home page.
    // To demonstrate keyboard, you would typically have an input field on a page
    // and when that input field is clicked, you call lvgl_keyboard_set_textarea.

    // Example of how to get a text area from a page and link it to the keyboard
    // This is a placeholder and should be integrated properly into your UI flow.
    // For instance, in wifi_bt_page.c, when a text input for SSID/password is created.
    /*
    lv_obj_t *some_textarea = lv_textarea_create(lv_scr_act());
    lv_obj_set_size(some_textarea, 300, 50);
    lv_obj_align(some_textarea, LV_ALIGN_TOP_MID, 0, 100);
    lv_textarea_set_placeholder_text(some_textarea, "Test keyboard...");
    lv_obj_add_event_cb(some_textarea, [](lv_event_t *e) {
        lv_event_code_t code = lv_event_get_code(e);
        if (code == LV_EVENT_CLICKED) {
            lvgl_keyboard_set_textarea(main_keyboard, lv_event_get_target(e));
        }
    }, LV_EVENT_ALL, NULL);
    */
}


