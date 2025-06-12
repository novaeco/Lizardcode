#ifndef GT911_TOUCH_H
#define GT911_TOUCH_H

#include <stdint.h>
#include <stdbool.h>
#include "driver/i2c.h"

#define GT911_I2C_ADDR 0x5D // GT911 I2C address

#define GT911_MAX_TOUCHES 5

typedef struct {
    uint8_t  finger_id;
    uint16_t x;
    uint16_t y;
    uint16_t area;
} gt911_touch_point_t;

typedef struct {
    uint8_t           touch_cnt;
    gt911_touch_point_t points[GT911_MAX_TOUCHES];
} gt911_touch_data_t;

/**
 * @brief Initialize GT911 touch controller.
 * @param i2c_num I2C port number.
 * @param sda_pin SDA pin number.
 * @param scl_pin SCL pin number.
 * @return ESP_OK on success, otherwise an error code.
 */
esp_err_t gt911_init(i2c_port_t i2c_num, gpio_num_t sda_pin, gpio_num_t scl_pin);

/**
 * @brief Read touch data from GT911.
 * @param data Pointer to gt911_touch_data_t structure to store touch data.
 * @return ESP_OK on success, otherwise an error code.
 */
esp_err_t gt911_read_touch_data(gt911_touch_data_t *data);

#endif // GT911_TOUCH_H


