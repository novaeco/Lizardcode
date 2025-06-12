#include "console_debug_page.h"
#include "lvgl.h"
#include "esp_log.h"
#include "i18n_service.h"

static const char *TAG = "CONSOLE_DEBUG_PAGE";

void create_console_debug_page(lv_obj_t *parent)
{
    lv_obj_t *console_debug_screen = lv_obj_create(parent);
    lv_obj_set_size(console_debug_screen, LV_HOR_RES, LV_VER_RES);
    lv_obj_set_flex_flow(console_debug_screen, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(console_debug_screen, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    lv_obj_t *title_label = lv_label_create(console_debug_screen);
    lv_label_set_text(title_label, i18n_service_get_string("console_debug_page_title"));
    lv_obj_set_style_text_font(title_label, &lv_font_montserrat_28, 0);

    lv_obj_t *log_area = lv_textarea_create(console_debug_screen);
    lv_obj_set_size(log_area, LV_HOR_RES - 40, LV_VER_RES / 2);
    lv_textarea_set_placeholder_text(log_area, i18n_service_get_string("system_logs_placeholder"));
    lv_obj_set_style_bg_color(log_area, lv_color_hex(0x222222), 0);
    lv_obj_set_style_text_color(log_area, lv_color_hex(0x00FF00), 0);

    lv_obj_t *info_label = lv_label_create(console_debug_screen);
    lv_label_set_text(info_label, i18n_service_get_string("system_info"));

    ESP_LOGI(TAG, "Console/Debug page created.");
}


