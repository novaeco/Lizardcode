#include "ui_manager.h"
#include "home_page.h"
#include "wifi_bt_page.h"
#include "file_explorer_page.h"
#include "settings_page.h"
#include "console_debug_page.h"
#include "esp_log.h"

static const char *TAG = "UI_MANAGER";

static lv_obj_t *pages[UI_PAGE_COUNT];
static ui_page_id_t current_page_id = UI_PAGE_HOME;
static lv_obj_t *base_screen_obj;

lv_obj_t *ui_manager_init(void)
{
    // Create a base screen for all pages
    base_screen_obj = lv_obj_create(NULL);
    lv_obj_set_size(base_screen_obj, LV_HOR_RES, LV_VER_RES);
    lv_obj_set_style_bg_color(base_screen_obj, lv_color_hex(0x1A1A1A), 0); // Dark background
    lv_obj_set_style_border_width(base_screen_obj, 0, 0);
    lv_obj_set_style_pad_all(base_screen_obj, 0, 0);

    // Create all pages
    pages[UI_PAGE_HOME] = lv_obj_create(base_screen_obj);
    lv_obj_set_size(pages[UI_PAGE_HOME], LV_HOR_RES, LV_VER_RES);
    lv_obj_set_style_bg_opa(pages[UI_PAGE_HOME], LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(pages[UI_PAGE_HOME], 0, 0);
    create_home_page(pages[UI_PAGE_HOME]);

    pages[UI_PAGE_WIFI_BT] = lv_obj_create(base_screen_obj);
    lv_obj_set_size(pages[UI_PAGE_WIFI_BT], LV_HOR_RES, LV_VER_RES);
    lv_obj_set_style_bg_opa(pages[UI_PAGE_WIFI_BT], LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(pages[UI_PAGE_WIFI_BT], 0, 0);
    create_wifi_bt_page(pages[UI_PAGE_WIFI_BT]);

    pages[UI_PAGE_FILE_EXPLORER] = lv_obj_create(base_screen_obj);
    lv_obj_set_size(pages[UI_PAGE_FILE_EXPLORER], LV_HOR_RES, LV_VER_RES);
    lv_obj_set_style_bg_opa(pages[UI_PAGE_FILE_EXPLORER], LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(pages[UI_PAGE_FILE_EXPLORER], 0, 0);
    create_file_explorer_page(pages[UI_PAGE_FILE_EXPLORER]);

    pages[UI_PAGE_SETTINGS] = lv_obj_create(base_screen_obj);
    lv_obj_set_size(pages[UI_PAGE_SETTINGS], LV_HOR_RES, LV_VER_RES);
    lv_obj_set_style_bg_opa(pages[UI_PAGE_SETTINGS], LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(pages[UI_PAGE_SETTINGS], 0, 0);
    create_settings_page(pages[UI_PAGE_SETTINGS]);

    pages[UI_PAGE_CONSOLE_DEBUG] = lv_obj_create(base_screen_obj);
    lv_obj_set_size(pages[UI_PAGE_CONSOLE_DEBUG], LV_HOR_RES, LV_VER_RES);
    lv_obj_set_style_bg_opa(pages[UI_PAGE_CONSOLE_DEBUG], LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(pages[UI_PAGE_CONSOLE_DEBUG], 0, 0);
    create_console_debug_page(pages[UI_PAGE_CONSOLE_DEBUG]);

    // Set initial page
    lv_scr_load_anim(base_screen_obj, LV_SCR_LOAD_ANIM_NONE, 0, 0, false);
    ui_manager_switch_page(UI_PAGE_HOME);

    ESP_LOGI(TAG, "UI Manager initialized. All pages created.");
    return base_screen_obj;
}

void ui_manager_switch_page(ui_page_id_t page_id)
{
    if (page_id >= UI_PAGE_COUNT) {
        ESP_LOGE(TAG, "Invalid page ID: %d", page_id);
        return;
    }

    // Hide current page
    lv_obj_add_flag(pages[current_page_id], LV_OBJ_FLAG_HIDDEN);

    // Show new page
    lv_obj_clear_flag(pages[page_id], LV_OBJ_FLAG_HIDDEN);
    current_page_id = page_id;

    ESP_LOGI(TAG, "Switched to page ID: %d", page_id);
}


