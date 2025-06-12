#include "lvgl_port.h"
#include "LovyanGFX.hpp"
#include "LGFX_Config.hpp"
#include "gt911.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char *TAG = "LVGL_PORT";

static LGFX lcd;

// LVGL display flush callback
static void lvgl_display_flush(lv_display_t *disp, const lv_area_t *area, uint8_t *px_map)
{
    lcd.pushImageDMA(area->x1, area->y1, area->x2 - area->x1 + 1, area->y2 - area->y1 + 1, (lgfx::rgb565_t *)px_map);
    lv_display_flush_ready(disp);
}

// LVGL touch input read callback
static void lvgl_touch_read(lv_indev_t *indev, lv_indev_data_t *data)
{
    gt911_touch_data_t touch_data;
    esp_err_t ret = gt911_read_touch_data(&touch_data);

    if (ret == ESP_OK && touch_data.touch_cnt > 0) {
        data->state = LV_INDEV_STATE_PRESSED;
        data->point.x = touch_data.points[0].x;
        data->point.y = touch_data.points[0].y;
    } else {
        data->state = LV_INDEV_STATE_RELEASED;
    }
}

void lvgl_port_init(void)
{
    // Initialize LovyanGFX
    lcd.init();
    lcd.setRotation(1); // Adjust rotation as needed
    lcd.setBrightness(128); // Set backlight brightness

    // Initialize I2C for GT911
    // These pin numbers are examples, please verify with your Waveshare board documentation
    esp_err_t ret = gt911_init(I2C_NUM_0, GPIO_NUM_8, GPIO_NUM_9); // SDA, SCL pins
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to initialize GT911: %s", esp_err_to_name(ret));
    }

    // Initialize LVGL
    lv_init();

    // Create LVGL display
    lv_display_t *disp = lv_display_create(lcd.width(), lcd.height());
    lv_display_set_flush_cb(disp, lvgl_display_flush);

    // Allocate draw buffers
    static lv_color_t *buf1 = (lv_color_t *)heap_caps_malloc(lcd.width() * 40 * sizeof(lv_color_t), MALLOC_CAP_DMA);
    static lv_color_t *buf2 = (lv_color_t *)heap_caps_malloc(lcd.width() * 40 * sizeof(lv_color_t), MALLOC_CAP_DMA);
    if (!buf1 || !buf2) {
        ESP_LOGE(TAG, "LVGL draw buffer malloc failed");
    }
    lv_display_set_buffers(disp, buf1, buf2, lcd.width() * 40 * sizeof(lv_color_t), LV_DISPLAY_RENDER_MODE_PARTIAL);

    // Create LVGL input device
    lv_indev_t *indev = lv_indev_create();
    lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER);
    lv_indev_set_read_cb(indev, lvgl_touch_read);

    ESP_LOGI(TAG, "LVGL port initialized.");
}

void lvgl_port_task(void *arg)
{
    while (1) {
        lv_task_handler();
        vTaskDelay(pdMS_TO_TICKS(5)); // LVGL task handler should be called every few milliseconds
    }
}


