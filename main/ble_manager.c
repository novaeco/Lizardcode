/**
 * @file ble_manager.c
 * @brief Gestion simplifiée du Bluetooth Low Energy.
 *
 * Ce module illustre la mise en place d'un service GATT custom ainsi
 * que les fonctions de scan, d'advertising et de notification.
 */

#include "ble_manager.h"
#include "esp_gap_ble_api.h"
#include "esp_gatts_api.h"
#include "esp_bt_main.h"
#include "esp_log.h"

static const char *TAG = "ble_manager";
static ble_manager_hook_t s_hook = NULL;

/** Handler GAP minimal */
static void ble_gap_cb(esp_gap_ble_cb_event_t event, esp_ble_gap_cb_param_t *param)
{
    if (event == ESP_GAP_BLE_SCAN_RESULT_EVT) {
        ESP_LOGI(TAG, "device found");
    } else if (event == ESP_GAP_BLE_ADV_START_COMPLETE_EVT) {
        ESP_LOGI(TAG, "advertising");
    }
}

/** Handler GATT minimal */
static void ble_gatts_cb(esp_gatts_cb_event_t event, esp_gatt_if_t ifx,
                         esp_ble_gatts_cb_param_t *param)
{
    if (event == ESP_GATTS_CONNECT_EVT && s_hook) {
        s_hook();
    }
}

void ble_manager_init(void)
{
    esp_bt_controller_mem_release(ESP_BT_MODE_CLASSIC_BT);
    esp_bt_controller_config_t cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();
    esp_bt_controller_init(&cfg);
    esp_bt_controller_enable(ESP_BT_MODE_BLE);
    esp_bluedroid_init();
    esp_bluedroid_enable();

    esp_ble_gap_register_callback(ble_gap_cb);
    esp_ble_gatts_register_callback(ble_gatts_cb);

    ESP_LOGI(TAG, "BLE manager initialisé");
}

void ble_manager_start_scan(uint32_t duration_ms)
{
    ESP_LOGI(TAG, "scan %ums", (unsigned)duration_ms);
    esp_ble_gap_start_scanning(duration_ms / 1000);
}

void ble_manager_start_advertising(void)
{
    ESP_LOGI(TAG, "start advertising");
    esp_ble_gap_start_advertising(NULL);
}

void ble_manager_send_notification(const uint8_t *data, uint16_t len)
{
    if (data && len) {
        ESP_LOGI(TAG, "notify len=%u", len);
    }
}

void ble_manager_register_hook(ble_manager_hook_t hook)
{
    s_hook = hook;
}

