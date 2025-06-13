/**
 * @file lcd_manager.c
 * @brief Implémentation du module lcd_manager
 */

#include "lcd_manager.h"
#include "touch_manager.h"
#include "expander_ch422g.h"
#include "driver/i2c.h"
#include "esp_log.h"
#include <string.h>

/** Nom du module pour les logs */
static const char *TAG = "lcd_manager";

/** Configuration LCD détectée */
static lcd_config_t s_lcd_cfg;

/**
 * @brief Initialisation du bus I2C utilisé pour la détection
 */
static esp_err_t lcd_i2c_init(void)
{
    static bool init = false;
    if (init) {
        return ESP_OK;
    }

    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = LCD_MANAGER_I2C_SDA,
        .scl_io_num = LCD_MANAGER_I2C_SCL,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = LCD_MANAGER_I2C_FREQ_HZ,
        .clk_flags = 0,
    };

    esp_err_t ret = i2c_param_config(LCD_MANAGER_I2C_PORT, &conf);
    if (ret == ESP_OK) {
        ret = i2c_driver_install(LCD_MANAGER_I2C_PORT, conf.mode, 0, 0, 0);
    }

    if (ret == ESP_OK) {
        init = true;
    }

    return ret;
}

void lcd_manager_init(void)
{
    memset(&s_lcd_cfg, 0, sizeof(s_lcd_cfg));

    lcd_i2c_init();

    expander_ch422g_t expander;
    bool has_expander = expander_ch422g_init(&expander, LCD_MANAGER_I2C_PORT);

    if (has_expander) {
        s_lcd_cfg.model  = LCD_MODEL_WAVESHARE_5B;
        s_lcd_cfg.width  = 1024;
        s_lcd_cfg.height = 600;
        ESP_LOGI(TAG, "Expander CH422G detecte - ecran 5B 1024x600");
    } else {
        s_lcd_cfg.model  = LCD_MODEL_WAVESHARE_7INCH;
        s_lcd_cfg.width  = 800;
        s_lcd_cfg.height = 480;
        ESP_LOGI(TAG, "Aucun expander detecte - ecran 7'' 800x480");
    }

    /* Detection du controleur tactile */
    touch_manager_init();

    ESP_LOGI(TAG, "LCD configure: %ux%u", s_lcd_cfg.width, s_lcd_cfg.height);
}

const lcd_config_t *lcd_manager_get_config(void)
{
    return &s_lcd_cfg;
}
