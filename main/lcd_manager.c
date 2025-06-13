#include "lcd_manager.h"
#include "touch_manager.h"

#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_lcd_panel_ops.h"
#include "esp_lcd_panel_rgb.h"

static const char *TAG = "lcd_manager";

/** Example RGB pin mapping for Waveshare boards. Adjust if required. */
#define LCD_PIN_PCLK   42
#define LCD_PIN_VSYNC  41
#define LCD_PIN_HSYNC  40
#define LCD_PIN_DE     39
static const int LCD_DATA_PINS[16] = {
    48, 47, 21, 14, 13, 12, 11, 10,
     9, 46,  3,  8, 16, 15,  7,  6
};

static esp_lcd_panel_handle_t s_panel = NULL;
static uint16_t s_width = 800;
static uint16_t s_height = 480;

esp_err_t lcd_manager_init(void)
{
    esp_err_t ret;

    /* Determine resolution from detected touch controller */
    switch (touch_manager_get_model()) {
    case TOUCH_MODEL_GT911:
        s_width = 800;
        s_height = 480;
        break;
    case TOUCH_MODEL_FT5436:
        s_width = 1024;
        s_height = 600;
        break;
    default:
        ESP_LOGW(TAG, "Unknown touch controller, defaulting to 800x480");
        s_width = 800;
        s_height = 480;
        break;
    }

    const esp_lcd_rgb_panel_config_t panel_config = {
        .clk_src = LCD_CLK_SRC_DEFAULT,
        .timings = {
            .pclk_hz = 16000000,
            .h_res = s_width,
            .v_res = s_height,
        },
        .data_width = 16,
        .hsync_gpio_num = LCD_PIN_HSYNC,
        .vsync_gpio_num = LCD_PIN_VSYNC,
        .de_gpio_num = LCD_PIN_DE,
        .pclk_gpio_num = LCD_PIN_PCLK,
        .data_gpio_nums = LCD_DATA_PINS,
        .flags.fb_in_psram = 1,
    };

    ret = esp_lcd_new_rgb_panel(&panel_config, &s_panel);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "esp_lcd_new_rgb_panel failed: %s", esp_err_to_name(ret));
        return ret;
    }

    ret = esp_lcd_panel_reset(s_panel);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "panel reset failed: %s", esp_err_to_name(ret));
        return ret;
    }

    ret = esp_lcd_panel_init(s_panel);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "panel init failed: %s", esp_err_to_name(ret));
        return ret;
    }

    esp_lcd_panel_disp_on_off(s_panel, true);
    ESP_LOGI(TAG, "LCD initialized (%ux%u)", s_width, s_height);
    return ESP_OK;
}

uint16_t lcd_manager_width(void)
{
    return s_width;
}

uint16_t lcd_manager_height(void)
{
    return s_height;
}

esp_lcd_panel_handle_t lcd_manager_panel(void)
{
    return s_panel;
}
