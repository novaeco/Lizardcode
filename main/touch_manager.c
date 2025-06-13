#include "touch_manager.h"

#include <string.h>
#include "driver/gpio.h"
#include "driver/i2c.h"
#include "esp_log.h"

static const char *TAG = "touch_manager";

#define GT911_ADDR1  0x5D
#define GT911_ADDR2  0x14
#define FT5436_ADDR  0x38

typedef struct {
    i2c_port_t port;
    gpio_num_t intr_gpio;
    gpio_num_t rst_gpio;
    touch_model_t model;
    uint16_t disp_w;
    uint16_t disp_h;
} touch_context_t;

static touch_context_t s_ctx = {
    .port = I2C_NUM_0,
    .intr_gpio = GPIO_NUM_NC,
    .rst_gpio = GPIO_NUM_NC,
    .model = TOUCH_MODEL_NONE,
};

static esp_err_t i2c_probe_addr(i2c_port_t port, uint8_t addr)
{
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (addr << 1) | I2C_MASTER_WRITE, true);
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(port, cmd, pdMS_TO_TICKS(100));
    i2c_cmd_link_delete(cmd);
    return ret;
}

static esp_err_t i2c_read16(uint8_t addr, uint16_t reg, uint8_t *data, size_t len)
{
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (addr << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(cmd, reg >> 8, true);
    i2c_master_write_byte(cmd, reg & 0xFF, true);
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (addr << 1) | I2C_MASTER_READ, true);
    if (len > 1) {
        i2c_master_read(cmd, data, len - 1, I2C_MASTER_ACK);
    }
    i2c_master_read_byte(cmd, data + len - 1, I2C_MASTER_NACK);
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(s_ctx.port, cmd, pdMS_TO_TICKS(100));
    i2c_cmd_link_delete(cmd);
    return ret;
}

static esp_err_t gt911_read_points(touch_point_t *points, size_t *num)
{
    uint8_t buf[1 + TOUCH_MAX_POINTS * 8];
    esp_err_t ret = i2c_read16(GT911_ADDR1, 0x814E, buf, sizeof(buf));
    if (ret != ESP_OK) {
        /* retry using second address */
        ret = i2c_read16(GT911_ADDR2, 0x814E, buf, sizeof(buf));
        if (ret != ESP_OK) {
            return ret;
        }
    }

    uint8_t touches = buf[0] & 0x0F;
    touches = touches > TOUCH_MAX_POINTS ? TOUCH_MAX_POINTS : touches;
    for (size_t i = 0; i < touches; i++) {
        uint8_t *src = &buf[1 + i * 8];
        points[i].x = (src[1] << 8) | src[0];
        points[i].y = (src[3] << 8) | src[2];
        points[i].id = src[4];
        points[i].pressed = true;
    }
    *num = touches;

    /* clear status */
    uint8_t zero = 0;
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (GT911_ADDR1 << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(cmd, 0x81, true);
    i2c_master_write_byte(cmd, 0x4E, true);
    i2c_master_write_byte(cmd, zero, true);
    i2c_master_stop(cmd);
    i2c_master_cmd_begin(s_ctx.port, cmd, pdMS_TO_TICKS(50));
    i2c_cmd_link_delete(cmd);

    return ESP_OK;
}

static esp_err_t ft5436_read_points(touch_point_t *points, size_t *num)
{
    uint8_t buf[1 + TOUCH_MAX_POINTS * 6];
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (FT5436_ADDR << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(cmd, 0x02, true);
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (FT5436_ADDR << 1) | I2C_MASTER_READ, true);
    i2c_master_read(cmd, buf, sizeof(buf), I2C_MASTER_LAST_NACK);
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(s_ctx.port, cmd, pdMS_TO_TICKS(100));
    i2c_cmd_link_delete(cmd);
    if (ret != ESP_OK) {
        return ret;
    }

    uint8_t touches = buf[0] & 0x0F;
    touches = touches > TOUCH_MAX_POINTS ? TOUCH_MAX_POINTS : touches;
    for (size_t i = 0; i < touches; i++) {
        uint8_t *src = &buf[1 + i * 6];
        points[i].id = src[0] >> 4;
        points[i].x = ((src[0] & 0x0F) << 8) | src[1];
        points[i].y = ((src[2] & 0x0F) << 8) | src[3];
        points[i].pressed = true;
    }
    *num = touches;
    return ESP_OK;
}

esp_err_t touch_manager_init(i2c_port_t port, gpio_num_t intr_gpio, gpio_num_t rst_gpio)
{
    s_ctx.port = port;
    s_ctx.intr_gpio = intr_gpio;
    s_ctx.rst_gpio = rst_gpio;

    s_ctx.model = TOUCH_MODEL_NONE;

    if (i2c_probe_addr(port, GT911_ADDR1) == ESP_OK || i2c_probe_addr(port, GT911_ADDR2) == ESP_OK) {
        s_ctx.model = TOUCH_MODEL_GT911;
    } else if (i2c_probe_addr(port, FT5436_ADDR) == ESP_OK) {
        s_ctx.model = TOUCH_MODEL_FT5436;
    } else {
        ESP_LOGE(TAG, "No supported touch controller found");
        return ESP_ERR_NOT_FOUND;
    }

    if (s_ctx.rst_gpio != GPIO_NUM_NC) {
        gpio_set_direction(s_ctx.rst_gpio, GPIO_MODE_OUTPUT);
        gpio_set_level(s_ctx.rst_gpio, 0);
        vTaskDelay(pdMS_TO_TICKS(10));
        gpio_set_level(s_ctx.rst_gpio, 1);
        vTaskDelay(pdMS_TO_TICKS(100));
    }

    if (s_ctx.intr_gpio != GPIO_NUM_NC) {
        gpio_set_direction(s_ctx.intr_gpio, GPIO_MODE_INPUT);
    }

    ESP_LOGI(TAG, "Detected %s touch controller", s_ctx.model == TOUCH_MODEL_GT911 ? "GT911" : "FT5436");
    return ESP_OK;
}

esp_err_t touch_manager_calibrate(uint16_t disp_w, uint16_t disp_h)
{
    s_ctx.disp_w = disp_w;
    s_ctx.disp_h = disp_h;
    return ESP_OK;
}

esp_err_t touch_manager_read(touch_point_t *points, size_t *num_points)
{
    if (!points || !num_points) {
        return ESP_ERR_INVALID_ARG;
    }

    *num_points = 0;

    if (s_ctx.model == TOUCH_MODEL_GT911) {
        return gt911_read_points(points, num_points);
    } else if (s_ctx.model == TOUCH_MODEL_FT5436) {
        return ft5436_read_points(points, num_points);
    }

    return ESP_ERR_INVALID_STATE;
}

touch_model_t touch_manager_get_model(void)
{
    return s_ctx.model;
}

