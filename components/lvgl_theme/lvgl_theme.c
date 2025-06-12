#include "lvgl_theme.h"
#include "esp_log.h"

static const char *TAG = "LVGL_THEME";

static lv_style_t style_default;
static lv_style_t style_btn;
static lv_style_t style_btn_pressed;
static lv_style_t style_textarea;
static lv_style_t style_list;

static void theme_apply_cb(lv_theme_t *th, lv_obj_t *obj)
{
    if (lv_obj_check_type(obj, &lv_obj_class)) {
        lv_obj_add_style(obj, &style_default, 0);
    }
    else if (lv_obj_check_type(obj, &lv_button_class)) {
        lv_obj_add_style(obj, &style_btn, 0);
        lv_obj_add_style(obj, &style_btn_pressed, LV_STATE_PRESSED);
    }
    else if (lv_obj_check_type(obj, &lv_textarea_class)) {
        lv_obj_add_style(obj, &style_textarea, 0);
    }
    else if (lv_obj_check_type(obj, &lv_list_class)) {
        lv_obj_add_style(obj, &style_list, 0);
    }
    // Add more widget types as needed
}

void lvgl_theme_init(void)
{
    // Default style (dark, modern, fluid)
    lv_style_init(&style_default);
    lv_style_set_bg_color(&style_default, lv_color_hex(0x1A1A1A)); // Dark background
    lv_style_set_text_color(&style_default, lv_color_hex(0xFFFFFF)); // White text
    lv_style_set_radius(&style_default, 5); // Slightly rounded corners
    lv_style_set_border_width(&style_default, 0);
    lv_style_set_pad_all(&style_default, 10);
    lv_style_set_anim_time(&style_default, 200); // Short animation time for fluidity

    // Button style
    lv_style_init(&style_btn);
    lv_style_set_bg_color(&style_btn, lv_color_hex(0x333333));
    lv_style_set_bg_opa(&style_btn, LV_OPA_COVER);
    lv_style_set_text_color(&style_btn, lv_color_hex(0xFFFFFF));
    lv_style_set_radius(&style_btn, 5);
    lv_style_set_border_width(&style_btn, 0);
    lv_style_set_pad_all(&style_btn, 10);
    lv_style_set_shadow_width(&style_btn, 0);

    // Button pressed style
    lv_style_init(&style_btn_pressed);
    lv_style_set_bg_color(&style_btn_pressed, lv_color_hex(0x555555));
    lv_style_set_bg_opa(&style_btn_pressed, LV_OPA_COVER);

    // Textarea style
    lv_style_init(&style_textarea);
    lv_style_set_bg_color(&style_textarea, lv_color_hex(0x222222));
    lv_style_set_text_color(&style_textarea, lv_color_hex(0xFFFFFF));
    lv_style_set_border_width(&style_textarea, 1);
    lv_style_set_border_color(&style_textarea, lv_color_hex(0x444444));
    lv_style_set_radius(&style_textarea, 5);
    lv_style_set_pad_all(&style_textarea, 5);

    // List style
    lv_style_init(&style_list);
    lv_style_set_bg_color(&style_list, lv_color_hex(0x222222));
    lv_style_set_border_width(&style_list, 0);
    lv_style_set_radius(&style_list, 5);
    lv_style_set_pad_all(&style_list, 5);

    // Create and set the theme
    lv_theme_t *th = lv_theme_default_init(lv_display_get_default(),
                                           lv_palette_main(LV_PALETTE_BLUE),
                                           lv_palette_main(LV_PALETTE_RED),
                                           false,
                                           lv_font_default());
    lv_theme_set_apply_cb(th, theme_apply_cb);
    lv_display_set_theme(lv_display_get_default(), th);

    ESP_LOGI(TAG, "LVGL custom theme initialized.");
}


