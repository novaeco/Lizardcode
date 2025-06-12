#include "bluetooth_service.h"
#include "esp_log.h"
#include "esp_bt.h"
#include "esp_gap_ble_api.h"
#include "esp_gatts_api.h"
#include "esp_bt_main.h"
#include "esp_gatt_defs.h"

static const char *TAG = "BLUETOOTH_SERVICE";

// Declare a GATT server interface
static esp_gatt_if_t gatts_if;

// Advertising parameters
static esp_ble_adv_params_t adv_params = {
    .adv_int_min        = 0x20, // 32 * 0.625ms = 20ms
    .adv_int_max        = 0x40, // 64 * 0.625ms = 40ms
    .adv_type           = ADV_TYPE_IND,
    .own_addr_type      = BLE_ADDR_TYPE_PUBLIC,
    .channel_map        = ADV_CHNL_ALL,
    .adv_filter_policy  = ADV_FILTER_ALLOW_SCAN_ANY_CON_ANY,
};

// Declare a GATT server callback function
static void gatts_profile_event_handler(esp_gatts_cb_event_t event, esp_gatt_if_t gatt_if, esp_ble_gatts_cb_param_t *param)
{
    switch (event) {
        case ESP_GATTS_REG_EVT:
            ESP_LOGI(TAG, "REGISTER_APP_EVT, status %d, app_id %d", param->reg.status, param->reg.app_id);
            gatts_if = gatt_if;
            break;
        default:
            break;
    }
}

// Declare a GAP callback function
static void gap_event_handler(esp_gap_ble_cb_event_t event, esp_ble_gap_cb_param_t *param)
{
    switch (event) {
        case ESP_GAP_BLE_ADV_DATA_SET_COMPLETE_EVT:
            ESP_LOGI(TAG, "ESP_GAP_BLE_ADV_DATA_SET_COMPLETE_EVT\n");
            esp_ble_gap_start_advertising(&adv_params);
            break;
        case ESP_GAP_BLE_ADV_START_COMPLETE_EVT:
            //advertising start complete event
            if (param->adv_start_cmpl.status != ESP_BT_STATUS_SUCCESS) {
                ESP_LOGE(TAG, "Advertising start failed\n");
            } else {
                ESP_LOGI(TAG, "Advertising start successfully\n");
            }
            break;
        default:
            break;
    }
}

// Advertising data
static uint8_t adv_data[] = {
    0x02, 0x01, 0x06, // Flags
    0x02, 0x0a, 0xeb, // Tx power
    0x03, 0x03, 0x00, 0x00, // 16-bit service UUID
    0x0f, 0x09, 'E', 'S', 'P', '3', '2', '-', 'S', '3', ' ', 'D', 'e', 'v', 'i', 'c', 'e' // Complete local name
};

esp_err_t bluetooth_service_init(void)
{
    esp_err_t ret;

    ESP_ERROR_CHECK(esp_bt_controller_mem_release(ESP_BT_MODE_CLASSIC_BT));

    esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();
    ret = esp_bt_controller_init(&bt_cfg);
    if (ret) {
        ESP_LOGE(TAG, "%s initialize controller failed: %s\n", __func__, esp_err_to_name(ret));
        return ret;
    }

    ret = esp_bt_controller_enable(ESP_BT_MODE_BLE);
    if (ret) {
        ESP_LOGE(TAG, "%s enable controller failed: %s\n", __func__, esp_err_to_name(ret));
        return ret;
    }

    ret = esp_bluedroid_init();
    if (ret) {
        ESP_LOGE(TAG, "%s init bluedroid failed: %s\n", __func__, esp_err_to_name(ret));
        return ret;
    }

    ret = esp_bluedroid_enable();
    if (ret) {
        ESP_LOGE(TAG, "%s enable bluedroid failed: %s\n", __func__, esp_err_to_name(ret));
        return ret;
    }

    esp_ble_gap_register_callback(gap_event_handler);
    esp_ble_gatts_register_callback(gatts_profile_event_handler);
    esp_ble_gatts_app_register(0); // Register an application ID

    esp_ble_gap_set_device_name("ESP32-S3 LVGL");
    esp_ble_gap_config_adv_data_raw(adv_data, sizeof(adv_data));

    ESP_LOGI(TAG, "Bluetooth service initialized.");
    return ESP_OK;
}

esp_err_t bluetooth_service_start_advertising(void)
{
    return esp_ble_gap_start_advertising(&adv_params);
}

esp_err_t bluetooth_service_stop_advertising(void)
{
    return esp_ble_gap_stop_advertising();
}


