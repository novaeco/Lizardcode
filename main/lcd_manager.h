#ifndef LCD_MANAGER_H
#define LCD_MANAGER_H

#include <stdint.h>
#include "esp_err.h"
#include "esp_lcd_panel_io.h"

#ifdef __cplusplus
extern "C" {
#endif

/** Supported screen resolutions */
typedef enum {
    LCD_RES_800_480,   /**< 7-inch display */
    LCD_RES_1024_600   /**< 5B 5-inch display */
} lcd_resolution_t;

/**
 * @brief Initialize the RGB LCD panel and autodetect its resolution.
 *
 * This helper configures the RGB interface and creates an
 * ``esp_lcd_panel_handle_t``. The display model is determined at runtime so
 * that the firmware works on both supported Waveshare boards.
 *
 * @return ESP_OK on success, an error code otherwise.
 */
esp_err_t lcd_manager_init(void);

/** @return Active screen width in pixels. */
uint16_t lcd_manager_width(void);

/** @return Active screen height in pixels. */
uint16_t lcd_manager_height(void);

/** @return Handle to the underlying LCD panel driver. */
esp_lcd_panel_handle_t lcd_manager_panel(void);

#ifdef __cplusplus
}
#endif

#endif /* LCD_MANAGER_H */
