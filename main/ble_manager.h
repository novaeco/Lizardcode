/**
 * @file ble_manager.h
 * @brief Gestion Bluetooth Low Energy avec service custom.
 */

#ifndef BLE_MANAGER_H
#define BLE_MANAGER_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/** Informations d'une notification */
typedef struct {
    uint16_t conn_id;      /**< Connexion cible */
    uint16_t attr_handle;  /**< Attribut concerné */
} ble_notify_cfg_t;

/** Prototype de callback pour intégrations futures */
typedef void (*ble_manager_hook_t)(void);

/** Initialise la pile BLE et le service GATT */
void ble_manager_init(void);

/** Lance un scan BLE actif */
void ble_manager_start_scan(uint32_t duration_ms);

/** Démarre l'advertising pour rendre l'appareil visible */
void ble_manager_start_advertising(void);

/** Envoie une notification sur la caractéristique custom */
void ble_manager_send_notification(const uint8_t *data, uint16_t len);

/** Enregistre un hook pour extensions externes */
void ble_manager_register_hook(ble_manager_hook_t hook);

#ifdef __cplusplus
}
#endif

#endif /* BLE_MANAGER_H */
