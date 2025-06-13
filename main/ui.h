/**
 * @file ui.h
 * @brief Module ui
 */

#ifndef UI_H
#define UI_H

#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl.h"

typedef enum {
    UI_LANG_FR,
    UI_LANG_EN
} ui_lang_t;

typedef enum {
    UI_THEME_LIGHT,
    UI_THEME_DARK
} ui_theme_t;

typedef enum {
    UI_READABILITY_NORMAL,
    UI_READABILITY_HIGH
} ui_readability_t;

void ui_init(void);
void ui_set_language(ui_lang_t lang);
void ui_set_theme(ui_theme_t theme);
void ui_set_readability(ui_readability_t level);
void ui_show_notification(const char *text);
void ui_update_diagnostics(void);

#ifdef __cplusplus
}
#endif

#endif /* UI_H */
