/**
 * @file touch_manager.h
 * @brief Module touch_manager
 */

#ifndef TOUCH_MANAGER_H
#define TOUCH_MANAGER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

/**
 * @brief Types de contrôleurs tactiles supportés
 */
typedef enum {
    TOUCH_DRIVER_UNKNOWN = 0, /**< Aucun contrôleur détecté */
    TOUCH_DRIVER_GT911,       /**< Contrôleur Goodix GT911 */
    TOUCH_DRIVER_FT5436       /**< Contrôleur FocalTech FT5436 */
} touch_driver_t;

/**
 * @brief Initialise la détection du contrôleur tactile
 */
void touch_manager_init(void);

/**
 * @brief Renvoie le type de contrôleur détecté
 */
touch_driver_t touch_manager_get_driver(void);

#ifdef __cplusplus
}
#endif

#endif /* TOUCH_MANAGER_H */
