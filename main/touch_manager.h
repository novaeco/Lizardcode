#ifndef TOUCH_MANAGER_H
#define TOUCH_MANAGER_H

#include <stdbool.h>
#include <stdint.h>
#include "esp_err.h"
#include "driver/i2c.h"

#ifdef __cplusplus
extern "C" {
#endif

#define TOUCH_MAX_POINTS 5

/** Supported touch controller models */
typedef enum {
    TOUCH_MODEL_NONE = 0,
    TOUCH_MODEL_GT911,
    TOUCH_MODEL_FT5436,
} touch_model_t;

/** Information for a single touch point */
typedef struct {
    uint16_t x;     /**< X coordinate */
    uint16_t y;     /**< Y coordinate */
    uint8_t id;     /**< Touch ID (for multitouch) */
    bool pressed;   /**< True if this point is active */
} touch_point_t;

/**
 * @brief Initialize and autodetect the touch controller.
 *
 * The I2C bus must be configured prior to calling this function. The interrupt
 * and reset pins can be set to ``GPIO_NUM_NC`` if unused.
 */
esp_err_t touch_manager_init(i2c_port_t port, gpio_num_t intr_gpio, gpio_num_t rst_gpio);

/** Read currently reported touch points. */
esp_err_t touch_manager_read(touch_point_t *points, size_t *num_points);

/** Get the detected controller model. */
touch_model_t touch_manager_get_model(void);

/** Apply calibration based on LCD size. */
esp_err_t touch_manager_calibrate(uint16_t disp_w, uint16_t disp_h);

#ifdef __cplusplus
}
#endif

#endif /* TOUCH_MANAGER_H */
