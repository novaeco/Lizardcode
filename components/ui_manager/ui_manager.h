#ifndef UI_MANAGER_H
#define UI_MANAGER_H

#include "lvgl.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    UI_PAGE_HOME,
    UI_PAGE_WIFI_BT,
    UI_PAGE_FILE_EXPLORER,
    UI_PAGE_SETTINGS,
    UI_PAGE_CONSOLE_DEBUG,
    UI_PAGE_COUNT
} ui_page_id_t;

/**
 * @brief Initialize the UI manager and create all pages.
 * @return The base screen object where all pages are created.
 */
lv_obj_t *ui_manager_init(void);

/**
 * @brief Switch to a specific UI page.
 * @param page_id The ID of the page to switch to.
 */
void ui_manager_switch_page(ui_page_id_t page_id);

#ifdef __cplusplus
}
#endif

#endif // UI_MANAGER_H


