#ifndef CONSOLE_DEBUG_PAGE_H
#define CONSOLE_DEBUG_PAGE_H

#include "lvgl.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Create the console/debug page UI.
 * @param parent The parent object for the console/debug page.
 */
void create_console_debug_page(lv_obj_t *parent);

#ifdef __cplusplus
}
#endif

#endif // CONSOLE_DEBUG_PAGE_H


