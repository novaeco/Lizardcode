#ifndef I18N_SERVICE_H
#define I18N_SERVICE_H

#include "lvgl.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    LANG_EN,
    LANG_FR,
    LANG_COUNT
} language_t;

/**
 * @brief Initialize the i18n service and load language data.
 */
void i18n_service_init(void);

/**
 * @brief Set the current language.
 * @param lang The language to set.
 */
void i18n_service_set_language(language_t lang);

/**
 * @brief Get a localized string.
 * @param key The key for the string.
 * @return The localized string.
 */
const char *i18n_service_get_string(const char *key);

#ifdef __cplusplus
}
#endif

#endif // I18N_SERVICE_H


