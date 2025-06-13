/**
 * @file wifi_manager.c
 * @brief Implémentation simplifiée du gestionnaire Wi-Fi.
 *
 * Ce module illustre la mise en place d'une gestion Wi-Fi avec scan,
 * connexion, reconnexion automatique et point d'accès de secours.
 * Les identifiants sont stockés dans le NVS chiffré.
 * Un serveur web de base protégé par login/mot de passe pourrait être
 * lancé ici ainsi que le support OTA.
 */

#include "wifi_manager.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_http_server.h"
#include "esp_ota_ops.h"

static const char *TAG = "wifi_manager";

/** Hook optionnel pour extensions (MQTT, Home Assistant, ...). */
static wifi_manager_hook_t s_hook = NULL;

/** Crédentials en cache */
static wifi_credentials_t s_credentials;

/** Vrai si l'on doit se reconnecter automatiquement */
static bool s_auto_reconnect = true;

/** Gestionnaire d'événements Wi-Fi */
static void wifi_event_handler(void *arg, esp_event_base_t base,
                               int32_t id, void *data)
{
    if (base == WIFI_EVENT && id == WIFI_EVENT_STA_DISCONNECTED) {
        ESP_LOGW(TAG, "disconnect event");
        if (s_auto_reconnect) {
            esp_wifi_connect();
        } else {
            wifi_manager_start_fallback_ap();
        }
    } else if (base == IP_EVENT && id == IP_EVENT_STA_GOT_IP) {
        ESP_LOGI(TAG, "connected");
        if (s_hook) {
            s_hook();
        }
    }
}

void wifi_manager_init(void)
{
    /* Initialisation NVS chiffrée */
    nvs_flash_secure_init();
    esp_netif_init();
    esp_event_loop_create_default();
    esp_netif_create_default_wifi_sta();
    esp_netif_create_default_wifi_ap();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&cfg);
    esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, wifi_event_handler, NULL);
    esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, wifi_event_handler, NULL);

    esp_wifi_set_mode(WIFI_MODE_STA);
    esp_wifi_start();

    ESP_LOGI(TAG, "wifi manager initialisé");
}

void wifi_manager_scan(void)
{
    wifi_scan_config_t cfg = {0};
    ESP_LOGI(TAG, "scan start");
    esp_wifi_scan_start(&cfg, true);
}

void wifi_manager_connect(const char *ssid, const char *password)
{
    wifi_config_t sta = {0};
    strncpy((char *)sta.sta.ssid, ssid, sizeof(sta.sta.ssid));
    strncpy((char *)sta.sta.password, password, sizeof(sta.sta.password));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &sta));
    ESP_LOGI(TAG, "connecting to %s", ssid);
    esp_wifi_connect();
}

void wifi_manager_enable_auto_reconnect(bool enable)
{
    s_auto_reconnect = enable;
    ESP_LOGI(TAG, "auto reconnect %s", enable ? "ON" : "OFF");
}

void wifi_manager_start_fallback_ap(void)
{
    wifi_config_t ap = {0};
    strcpy((char *)ap.ap.ssid, "fallback");
    strcpy((char *)ap.ap.password, "12345678");
    ap.ap.ssid_len = strlen("fallback");
    ap.ap.max_connection = 1;
    ap.ap.authmode = WIFI_AUTH_WPA_WPA2_PSK;

    ESP_LOGI(TAG, "starting fallback AP");
    esp_wifi_set_mode(WIFI_MODE_AP);
    esp_wifi_set_config(WIFI_IF_AP, &ap);
}

void wifi_manager_set_credentials(const wifi_credentials_t *cred)
{
    if (!cred) {
        return;
    }
    nvs_handle_t nvs;
    if (nvs_open_from_partition("nvs", "wifi", NVS_READWRITE, &nvs) == ESP_OK) {
        nvs_set_str(nvs, "ssid", cred->ssid);
        nvs_set_str(nvs, "pass", cred->password);
        nvs_set_u8(nvs, "ent", cred->wpa2_enterprise);
        nvs_commit(nvs);
        nvs_close(nvs);
        s_credentials = *cred;
        ESP_LOGI(TAG, "credentials saved");
    }
}

bool wifi_manager_get_credentials(wifi_credentials_t *cred)
{
    if (!cred) {
        return false;
    }
    nvs_handle_t nvs;
    if (nvs_open_from_partition("nvs", "wifi", NVS_READONLY, &nvs) != ESP_OK) {
        return false;
    }
    size_t len1 = sizeof(cred->ssid);
    size_t len2 = sizeof(cred->password);
    if (nvs_get_str(nvs, "ssid", cred->ssid, &len1) != ESP_OK ||
        nvs_get_str(nvs, "pass", cred->password, &len2) != ESP_OK) {
        nvs_close(nvs);
        return false;
    }
    uint8_t ent = 0;
    nvs_get_u8(nvs, "ent", &ent);
    cred->wpa2_enterprise = ent;
    nvs_close(nvs);
    return true;
}

void wifi_manager_trigger_ota_rollback(void)
{
    ESP_LOGW(TAG, "trigger OTA rollback");
    esp_ota_mark_app_invalid_rollback_and_reboot();
}

void wifi_manager_register_hook(wifi_manager_hook_t hook)
{
    s_hook = hook;
}

