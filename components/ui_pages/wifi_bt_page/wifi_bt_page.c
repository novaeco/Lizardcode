#include "wifi_bt_page.h"
#include "lvgl.h"
#include "esp_log.h"
#include "i18n_service.h"

static const char *TAG = "WIFI_BT_PAGE";

void create_wifi_bt_page(lv_obj_t *parent)
{
    lv_obj_t *wifi_bt_screen = lv_obj_create(parent);
    lv_obj_set_size(wifi_bt_screen, LV_HOR_RES, LV_VER_RES);
    lv_obj_set_flex_flow(wifi_bt_screen, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(wifi_bt_screen, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    lv_obj_t *title_label = lv_label_create(wifi_bt_screen);
    lv_label_set_text(title_label, i18n_service_get_string("wifi_bt_page_title"));
    lv_obj_set_style_text_font(title_label, &lv_font_montserrat_28, 0);

    lv_obj_t *wifi_scan_btn = lv_btn_create(wifi_bt_screen);
    lv_obj_set_size(wifi_scan_btn, 200, 50);
    lv_obj_t *wifi_scan_label = lv_label_create(wifi_scan_btn);
    lv_label_set_text(wifi_scan_label, i18n_service_get_string("scan_wifi_btn"));
    lv_obj_center(wifi_scan_label);

    lv_obj_t *bt_scan_btn = lv_btn_create(wifi_bt_screen);
    lv_obj_set_size(bt_scan_btn, 200, 50);
    lv_obj_t *bt_scan_label = lv_label_create(bt_scan_btn);
    lv_label_set_text(bt_scan_label, i18n_service_get_string("scan_bluetooth_btn"));
    lv_obj_center(bt_scan_label);

    ESP_LOGI(TAG, "Wi-Fi/Bluetooth page created.");
}


