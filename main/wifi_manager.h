/**
 * @file wifi_manager.h
 * @brief Gestion du Wi-Fi (scan, connexion et AP de secours).
 */

#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <stdbool.h>
#include "esp_wifi_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/** Identifiants Wi-Fi stockés en NVS */
typedef struct {
    char ssid[32];        /**< SSID du réseau */
    char password[64];    /**< Mot de passe */
    bool wpa2_enterprise; /**< Mode WPA2-Enterprise */
} wifi_credentials_t;

/** Prototype de callback pour intégrations externes */
typedef void (*wifi_manager_hook_t)(void);

/** Initialise la pile Wi-Fi et les handlers d'événements */
void wifi_manager_init(void);

/** Lance un scan des points d'accès disponibles */
void wifi_manager_scan(void);

/** Connecte au réseau indiqué */
void wifi_manager_connect(const char *ssid, const char *password);

/** Active ou non la reconnexion automatique */
void wifi_manager_enable_auto_reconnect(bool enable);

/** Démarre un AP de secours en cas d'échec */
void wifi_manager_start_fallback_ap(void);

/** Sauvegarde les identifiants dans le NVS chiffré */
void wifi_manager_set_credentials(const wifi_credentials_t *cred);

/** Récupère les identifiants sauvegardés */
bool wifi_manager_get_credentials(wifi_credentials_t *cred);

/** Déclenche un rollback OTA */
void wifi_manager_trigger_ota_rollback(void);

/** Enregistre un hook pour extensions (MQTT, Home Assistant, ...) */
void wifi_manager_register_hook(wifi_manager_hook_t hook);

#ifdef __cplusplus
}
#endif

#endif /* WIFI_MANAGER_H */
