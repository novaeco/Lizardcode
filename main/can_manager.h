/**
 * @file can_manager.h
 * @brief Gestion simplifiée du contrôleur CAN
 */

#ifndef CAN_MANAGER_H
#define CAN_MANAGER_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/** Configuration de l'interface CAN */
typedef struct {
    uint32_t bitrate;      /**< Vitesse en bit/s */
    bool termination;      /**< Activation de la résistance de terminaison */
} can_config_t;

/** Représentation minimale d'une trame CAN */
typedef struct {
    uint32_t id;           /**< Identifiant de la trame */
    uint8_t  dlc;          /**< Nombre d'octets de data */
    uint8_t  data[8];      /**< Données */
} can_frame_t;

/**
 * @brief Initialisation du bus CAN
 *
 * @param config Paramètres de configuration (NULL pour valeurs par défaut)
 */
void can_manager_init(const can_config_t *config);

/**
 * @brief Changement dynamique du bitrate
 *
 * @param bitrate Nouvelle valeur en bit/s
 */
void can_manager_set_bitrate(uint32_t bitrate);

/**
 * @brief Active ou désactive la terminaison 120Ω
 */
void can_manager_enable_termination(bool enable);

/**
 * @brief Trace une trame sur la sortie de diagnostic
 */
void can_manager_log_frame(const can_frame_t *frame);

#ifdef __cplusplus
}
#endif

#endif /* CAN_MANAGER_H */
