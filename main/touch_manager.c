/**
 * @file touch_manager.c
 * @brief Implémentation du module touch_manager
 */

#include "touch_manager.h"
#include "lcd_manager.h"
#include "driver/i2c.h"
#include "esp_log.h"

/** Nom du module pour les logs */
static const char *TAG = "touch_manager";

/** Contrôleur détecté */
static touch_driver_t s_driver = TOUCH_DRIVER_UNKNOWN;

/**
 * @brief Probe rapide d'une adresse I2C
 */
static bool probe_addr(int port, uint8_t addr)
{
    return i2c_master_write_read_device(port, addr, NULL, 0, NULL, 0, pdMS_TO_TICKS(50)) == ESP_OK;
}

void touch_manager_init(void)
{
    int port = LCD_MANAGER_I2C_PORT;

    if (probe_addr(port, 0x5d) || probe_addr(port, 0x14)) {
        s_driver = TOUCH_DRIVER_GT911;
        ESP_LOGI(TAG, "Controleur tactile GT911 detecte");
    } else if (probe_addr(port, 0x38)) {
        s_driver = TOUCH_DRIVER_FT5436;
        ESP_LOGI(TAG, "Controleur tactile FT5436 detecte");
    } else {
        s_driver = TOUCH_DRIVER_UNKNOWN;
        ESP_LOGW(TAG, "Aucun controleur tactile detecte");
    }
}

touch_driver_t touch_manager_get_driver(void)
{
    return s_driver;
}
