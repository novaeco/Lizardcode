#include "gt911.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char *TAG = "GT911_DRIVER";
static i2c_port_t i2c_master_port;

/**
 * @brief Write data to GT911 register.
 */
static esp_err_t gt911_write_reg(uint16_t reg, uint8_t *data, size_t len)
{
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (GT911_I2C_ADDR << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(cmd, (reg >> 8) & 0xFF, true); // MSB of register address
    i2c_master_write_byte(cmd, reg & 0xFF, true);     // LSB of register address
    i2c_master_write(cmd, data, len, true);
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(i2c_master_port, cmd, 1000 / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(cmd);
    return ret;
}

/**
 * @brief Read data from GT911 register.
 */
static esp_err_t gt911_read_reg(uint16_t reg, uint8_t *data, size_t len)
{
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (GT911_I2C_ADDR << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(cmd, (reg >> 8) & 0xFF, true); // MSB of register address
    i2c_master_write_byte(cmd, reg & 0xFF, true);     // LSB of register address
    esp_err_t ret = i2c_master_cmd_begin(i2c_master_port, cmd, 1000 / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(cmd);

    if (ret != ESP_OK) {
        return ret;
    }

    cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (GT911_I2C_ADDR << 1) | I2C_MASTER_READ, true);
    i2c_master_read(cmd, data, len, I2C_MASTER_LAST_NACK);
    i2c_master_stop(cmd);
    ret = i2c_master_cmd_begin(i2c_master_port, cmd, 1000 / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(cmd);
    return ret;
}

esp_err_t gt911_init(i2c_port_t i2c_num, gpio_num_t sda_pin, gpio_num_t scl_pin)
{
    i2c_master_port = i2c_num;

    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = sda_pin,
        .scl_io_num = scl_pin,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = 400000, // 400KHz
    };

    esp_err_t ret = i2c_param_config(i2c_master_port, &conf);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "I2C param config failed: %s", esp_err_to_name(ret));
        return ret;
    }

    ret = i2c_driver_install(i2c_master_port, conf.mode, 0, 0, 0);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "I2C driver install failed: %s", esp_err_to_name(ret));
        return ret;
    }

    ESP_LOGI(TAG, "GT911 initialized successfully.");
    return ESP_OK;
}

esp_err_t gt911_read_touch_data(gt911_touch_data_t *data)
{
    uint8_t buf[GT911_MAX_TOUCHES * 8 + 1]; // 1 byte for touch status, 8 bytes per touch point
    esp_err_t ret = gt911_read_reg(0x8140, buf, sizeof(buf)); // Touch status register
    if (ret != ESP_OK) {
        return ret;
    }

    uint8_t touch_status = buf[0];
    data->touch_cnt = touch_status & 0x0F; // Number of touch points

    if (data->touch_cnt > GT911_MAX_TOUCHES) {
        data->touch_cnt = GT911_MAX_TOUCHES; // Cap at max touches
    }

    if (data->touch_cnt > 0) {
        // Clear touch status register after reading
        uint8_t clear_status = 0x00;
        gt911_write_reg(0x8140, &clear_status, 1);

        for (int i = 0; i < data->touch_cnt; i++) {
            // Touch point data starts from 0x814F
            // Each touch point is 8 bytes: x (2), y (2), size (2), id (1), reserved (1)
            uint16_t reg_addr = 0x814F + i * 8;
            ret = gt911_read_reg(reg_addr, &buf[1 + i * 8], 8); // Read 8 bytes for each touch point
            if (ret != ESP_OK) {
                return ret;
            }

            data->points[i].x = ((uint16_t)buf[1 + i * 8 + 1] << 8) | buf[1 + i * 8];
            data->points[i].y = ((uint16_t)buf[1 + i * 8 + 3] << 8) | buf[1 + i * 8 + 2];
            data->points[i].area = ((uint16_t)buf[1 + i * 8 + 5] << 8) | buf[1 + i * 8 + 4];
            data->points[i].finger_id = buf[1 + i * 8 + 7];
        }
    }

    return ESP_OK;
}


