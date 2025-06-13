/**
 * @file gpio_manager.h
 * @brief Gestion des entrées/sorties avec remapping dynamique
 */

#ifndef GPIO_MANAGER_H
#define GPIO_MANAGER_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/** Nombre maximum de GPIO virtuels exposés */
#define GPIO_MANAGER_MAX 16

/**
 * @brief Initialisation du gestionnaire GPIO
 */
void gpio_manager_init(void);

/**
 * @brief Lecture d'un GPIO logique
 */
bool gpio_manager_read(uint8_t index);

/**
 * @brief Écriture d'un GPIO logique
 */
void gpio_manager_write(uint8_t index, bool level);

/**
 * @brief Remappe dynamiquement un GPIO logique vers une broche physique
 */
void gpio_manager_remap(uint8_t index, int gpio_num);

/**
 * @brief Affiche le menu de remapping dans l'UI
 */
void gpio_manager_show_remap_menu(void);

#ifdef __cplusplus
}
#endif

#endif /* GPIO_MANAGER_H */
