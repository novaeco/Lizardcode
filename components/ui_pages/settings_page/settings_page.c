#include "settings_page.h"
#include "lvgl.h"
#include "esp_log.h"
#include "i18n_service.h"

static const char *TAG = "SETTINGS_PAGE";

void create_settings_page(lv_obj_t *parent)
{
    lv_obj_t *settings_screen = lv_obj_create(parent);
    lv_obj_set_size(settings_screen, LV_HOR_RES, LV_VER_RES);
    lv_obj_set_flex_flow(settings_screen, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(settings_screen, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    lv_obj_t *title_label = lv_label_create(settings_screen);
    lv_label_set_text(title_label, i18n_service_get_string("settings_page_title"));
    lv_obj_set_style_text_font(title_label, &lv_font_montserrat_28, 0);

    lv_obj_t *theme_btn = lv_btn_create(settings_screen);
    lv_obj_set_size(theme_btn, 200, 50);
    lv_obj_t *theme_label = lv_label_create(theme_btn);
    lv_label_set_text(theme_label, i18n_service_get_string("change_theme_btn"));
    lv_obj_center(theme_label);

    lv_obj_t *lang_btn = lv_btn_create(settings_screen);
    lv_obj_set_size(lang_btn, 200, 50);
    lv_obj_t *lang_label = lv_label_create(lang_btn);
    lv_label_set_text(lang_label, i18n_service_get_string("change_language_btn"));
    lv_obj_center(lang_label);

    ESP_LOGI(TAG, "Settings page created.");
}


