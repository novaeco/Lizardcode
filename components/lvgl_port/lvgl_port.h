#ifndef LVGL_PORT_H
#define LVGL_PORT_H

#include "lvgl.h"

#ifdef __cplusplus
extern "C" { // Only necessary if this header is included in C++ files
#endif

/**
 * @brief Initialize LVGL and its display/input drivers.
 */
void lvgl_port_init(void);

/**
 * @brief Handle LVGL tasks (should be called periodically).
 */
void lvgl_port_task(void *arg);

#ifdef __cplusplus
}
#endif

#endif // LVGL_PORT_H


