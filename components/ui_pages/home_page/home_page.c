#include "home_page.h"
#include "lvgl.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "i18n_service.h"

static const char *TAG = "HOME_PAGE";
static lv_obj_t *time_label;

static void update_time_task(void *arg)
{
    char time_str[32];
    while (1) {
        time_t now;
        struct tm timeinfo;
        time(&now);
        localtime_r(&now, &timeinfo);
        strftime(time_str, sizeof(time_str), "%H:%M:%S", &timeinfo);
        lv_label_set_text(time_label, time_str);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void create_home_page(lv_obj_t *parent)
{
    lv_obj_t *home_screen = lv_obj_create(parent);
    lv_obj_set_size(home_screen, LV_HOR_RES, LV_VER_RES);
    lv_obj_set_flex_flow(home_screen, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(home_screen, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    // Title
    lv_obj_t *title_label = lv_label_create(home_screen);
    lv_label_set_text(title_label, i18n_service_get_string("app_title"));
    lv_obj_set_style_text_font(title_label, &lv_font_montserrat_28, 0);
    lv_obj_set_style_text_align(title_label, LV_TEXT_ALIGN_CENTER, 0);

    // Time
    time_label = lv_label_create(home_screen);
    lv_obj_set_style_text_font(time_label, &lv_font_montserrat_24, 0);
    lv_obj_set_style_text_align(time_label, LV_TEXT_ALIGN_CENTER, 0);
    xTaskCreate(update_time_task, "update_time", 2048, NULL, 5, NULL);

    // Logo (placeholder for now)
    lv_obj_t *logo_img = lv_img_create(home_screen);
    // lv_img_set_src(logo_img, &your_logo_image_variable); // Uncomment and set your logo image
    lv_obj_set_size(logo_img, 100, 100); // Example size
    lv_obj_set_style_bg_color(logo_img, lv_color_hex(0x333333), 0); // Placeholder background
    lv_obj_set_style_radius(logo_img, 10, 0);

    ESP_LOGI(TAG, "Home page created.");
}


