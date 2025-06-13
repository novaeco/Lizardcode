/**
 * @file battery_manager.h
 * @brief Suivi de l'état de la batterie
 */

#ifndef BATTERY_MANAGER_H
#define BATTERY_MANAGER_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initialisation du suivi batterie
 */
void battery_manager_init(int adc_channel, int gpio_test);

/**
 * @brief Retourne le niveau de charge en pourcentage
 */
uint8_t battery_manager_get_level(void);

/**
 * @brief Lance un test de présence via un GPIO dédié
 */
bool battery_manager_gpio_test(void);

#ifdef __cplusplus
}
#endif

#endif /* BATTERY_MANAGER_H */
