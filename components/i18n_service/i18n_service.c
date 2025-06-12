#include "i18n_service.h"
#include "esp_log.h"
#include <string.h>

static const char *TAG = "I18N_SERVICE";

// Simple dictionary for English and French
typedef struct {
    const char *key;
    const char *en_value;
    const char *fr_value;
} i18n_entry_t;

static const i18n_entry_t i18n_dictionary[] = {
    {"app_title", "ESP32-S3 LVGL Project", "Projet LVGL ESP32-S3"},
    {"home_page_title", "Home", "Accueil"},
    {"wifi_bt_page_title", "Wi-Fi / Bluetooth", "Wi-Fi / Bluetooth"},
    {"file_explorer_page_title", "File Explorer", "Explorateur de Fichiers"},
    {"settings_page_title", "System Settings", "Paramètres Système"},
    {"console_debug_page_title", "Console / Debug", "Console / Débogage"},
    {"scan_wifi_btn", "Scan Wi-Fi", "Scanner Wi-Fi"},
    {"scan_bluetooth_btn", "Scan Bluetooth", "Scanner Bluetooth"},
    {"browse_spiffs_btn", "Browse SPIFFS", "Parcourir SPIFFS"},
    {"browse_sd_btn", "Browse SD Card", "Parcourir Carte SD"},
    {"change_theme_btn", "Change Theme", "Changer de Thème"},
    {"change_language_btn", "Change Language", "Changer de Langue"},
    {"system_info", "System Information: ESP32-S3, LVGL 9.2.2", "Informations Système: ESP32-S3, LVGL 9.2.2"},
    {"enter_text_placeholder", "Enter text here...", "Entrez du texte ici..."},
    {"system_logs_placeholder", "System logs will appear here...", "Les journaux système apparaîtront ici..."},
    // Add more strings as needed
    {NULL, NULL, NULL} // Sentinel
};

static language_t current_language = LANG_EN; // Default language

void i18n_service_init(void)
{
    ESP_LOGI(TAG, "i18n service initialized.");
}

void i18n_service_set_language(language_t lang)
{
    if (lang >= LANG_COUNT) {
        ESP_LOGE(TAG, "Invalid language ID: %d", lang);
        return;
    }
    current_language = lang;
    ESP_LOGI(TAG, "Language set to %s", (lang == LANG_EN) ? "English" : "French");
    // In a real application, you would typically refresh the UI here to apply new language
}

const char *i18n_service_get_string(const char *key)
{
    for (int i = 0; i18n_dictionary[i].key != NULL; i++) {
        if (strcmp(i18n_dictionary[i].key, key) == 0) {
            if (current_language == LANG_EN) {
                return i18n_dictionary[i].en_value;
            } else if (current_language == LANG_FR) {
                return i18n_dictionary[i].fr_value;
            }
        }
    }
    ESP_LOGW(TAG, "String not found for key: %s", key);
    return key; // Return key itself if not found
}


