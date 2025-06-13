/**
 * @file expander_ch422g.c
 * @brief Implémentation du module expander_ch422g
 */

#include "expander_ch422g.h"
#include "driver/i2c.h"
#include "esp_log.h"

/** Nom du module */
static const char *TAG = "ch422g";

/* Adresses I2C des "registres" du CH422G */
#define CH422G_REG_WR_SET (0x48 >> 1)
#define CH422G_REG_WR_OC  (0x46 >> 1)
#define CH422G_REG_WR_IO  (0x70 >> 1)
#define CH422G_REG_RD_IO  (0x4D >> 1)

/* Valeurs par defaut */
#define CH422G_WR_SET_DEFAULT 0x01
#define CH422G_WR_OC_DEFAULT  0x0F
#define CH422G_WR_IO_DEFAULT  0xFF

/* Fonctions internes d'acces I2C */
static esp_err_t ch422g_write_reg(const expander_ch422g_t *dev, uint8_t reg, uint8_t val)
{
    return i2c_master_write_to_device(dev->port, reg, &val, 1, pdMS_TO_TICKS(50));
}

static esp_err_t ch422g_read_reg(const expander_ch422g_t *dev, uint8_t reg, uint8_t *val)
{
    return i2c_master_read_from_device(dev->port, reg, val, 1, pdMS_TO_TICKS(50));
}

bool expander_ch422g_init(expander_ch422g_t *dev, int port)
{
    if (!dev) {
        return false;
    }

    dev->port    = port;
    dev->address = CH422G_REG_WR_SET; /* premiere adresse connue */
    dev->wr_set  = CH422G_WR_SET_DEFAULT;
    dev->wr_oc   = CH422G_WR_OC_DEFAULT;
    dev->wr_io   = CH422G_WR_IO_DEFAULT;

    /* tentative d'ecriture pour verifier la presence du composant */
    esp_err_t ret = ch422g_write_reg(dev, CH422G_REG_WR_SET, dev->wr_set);
    if (ret != ESP_OK) {
        dev->present = false;
        ESP_LOGW(TAG, "CH422G absent");
        return false;
    }

    /* configuration par defaut */
    ch422g_write_reg(dev, CH422G_REG_WR_OC,  dev->wr_oc);
    ch422g_write_reg(dev, CH422G_REG_WR_IO,  dev->wr_io);

    dev->present = true;
    ESP_LOGI(TAG, "CH422G detecte sur le port %d", port);
    return true;
}

esp_err_t expander_ch422g_set_all_output(expander_ch422g_t *dev)
{
    if (!dev || !dev->present) {
        return ESP_ERR_INVALID_ARG;
    }

    dev->wr_set |= 0x01; /* BIT_IO_OE */
    return ch422g_write_reg(dev, CH422G_REG_WR_SET, dev->wr_set);
}

esp_err_t expander_ch422g_write(expander_ch422g_t *dev, uint16_t value)
{
    if (!dev || !dev->present) {
        return ESP_ERR_INVALID_ARG;
    }

    uint8_t oc = (value >> 8) & 0x0F;
    uint8_t io = value & 0xFF;

    esp_err_t ret1 = ch422g_write_reg(dev, CH422G_REG_WR_OC, oc);
    esp_err_t ret2 = ch422g_write_reg(dev, CH422G_REG_WR_IO, io);

    if (ret1 == ESP_OK) dev->wr_oc = oc;
    if (ret2 == ESP_OK) dev->wr_io = io;

    return (ret1 != ESP_OK) ? ret1 : ret2;
}

esp_err_t expander_ch422g_read(expander_ch422g_t *dev, uint8_t *value)
{
    if (!dev || !dev->present || !value) {
        return ESP_ERR_INVALID_ARG;
    }

    return ch422g_read_reg(dev, CH422G_REG_RD_IO, value);
}
