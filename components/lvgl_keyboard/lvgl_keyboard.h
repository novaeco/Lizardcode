#ifndef LVGL_KEYBOARD_H
#define LVGL_KEYBOARD_H

#include "lvgl.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Create an AZERTY virtual keyboard.
 * @param parent The parent object for the keyboard.
 * @return A pointer to the created keyboard object.
 */
lv_obj_t *lvgl_keyboard_create(lv_obj_t *parent);

/**
 * @brief Set the target text area for the keyboard.
 * @param kb The keyboard object.
 * @param ta The text area object to associate with the keyboard.
 */
void lvgl_keyboard_set_textarea(lv_obj_t *kb, lv_obj_t *ta);

#ifdef __cplusplus
}
#endif

#endif // LVGL_KEYBOARD_H


