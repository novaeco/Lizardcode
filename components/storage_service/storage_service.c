#include "storage_service.h"
#include "esp_log.h"
#include "esp_spiffs.h"
#include "esp_vfs_fat.h"
#include "sdmmc_host.h"
#include "driver/sdmmc_host.h"
#include "driver/sdmmc_defs.h"
#include "sdmmc_cmd.h"
#include <stdio.h>
#include <string.h>

static const char *TAG = "STORAGE_SERVICE";

// Mount points
#define SPIFFS_MOUNT_POINT "/spiffs"
#define SD_MOUNT_POINT     "/sdcard"

esp_err_t storage_service_init(void)
{
    esp_err_t ret;

    // Initialize SPIFFS
    ESP_LOGI(TAG, "Initializing SPIFFS");
    esp_vfs_spiffs_conf_t spiffs_conf = {
      .base_path = SPIFFS_MOUNT_POINT,
      .partition_label = NULL,
      .max_files = 5,   // Max number of files that can be open at the same time
      .format_if_mount_failed = true
    };

    ret = esp_vfs_spiffs_register(&spiffs_conf);

    if (ret != ESP_OK) {
        if (ret == ESP_FAIL) {
            ESP_LOGE(TAG, "Failed to mount or format SPIFFS");
        } else if (ret == ESP_ERR_NOT_FOUND) {
            ESP_LOGE(TAG, "Failed to find SPIFFS partition");
        } else {
            ESP_LOGE(TAG, "Failed to initialize SPIFFS (%s)", esp_err_to_name(ret));
        }
        return ret;
    }

    size_t total = 0, used = 0;
    ret = esp_spiffs_info(spiffs_conf.partition_label, &total, &used);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to get SPIFFS partition information (%s)", esp_err_to_name(ret));
    } else {
        ESP_LOGI(TAG, "Partition size: total: %d, used: %d", total, used);
    }

    // Initialize SD card
    ESP_LOGI(TAG, "Initializing SD card");
    sdmmc_host_t host = SDMMC_HOST_DEFAULT();
    sdmmc_slot_config_t slot_config = SDMMC_SLOT_CONFIG_DEFAULT();

    // For ESP32-S3, you might need to adjust these pins based on your board
    // Example for 4-bit mode:
    slot_config.width = 4;
    slot_config.clk = GPIO_NUM_36; // Example, verify with your board
    slot_config.cmd = GPIO_NUM_35; // Example, verify with your board
    slot_config.d0 = GPIO_NUM_37; // Example, verify with your board
    slot_config.d1 = GPIO_NUM_38; // Example, verify with your board
    slot_config.d2 = GPIO_NUM_33; // Example, verify with your board
    slot_config.d3 = GPIO_NUM_34; // Example, verify with your board

    esp_vfs_fat_sdmmc_mount_config_t mount_config = {
        .format_if_mount_failed = true,
        .max_files = 5,
        .allocation_unit_size = 16 * 1024
    };

    sdmmc_card_t *card;
    ret = esp_vfs_fat_sdmmc_mount(SD_MOUNT_POINT, &host, &slot_config, &mount_config, &card);

    if (ret != ESP_OK) {
        if (ret == ESP_FAIL) {
            ESP_LOGE(TAG, "Failed to mount FATFS. If you want to format the card, set format_if_mount_failed = true.");
        } else {
            ESP_LOGE(TAG, "Failed to initialize the card (%s). Make sure SD card lines have pull-up resistors in place.", esp_err_to_name(ret));
        }
        return ret;
    }

    ESP_LOGI(TAG, "SD card initialized.");
    sdmmc_card_print_info(stdout, card);

    return ESP_OK;
}

esp_err_t storage_service_list_dir(storage_type_t storage_type, const char *path, void *file_list, void *dir_list)
{
    const char *base_path = (storage_type == STORAGE_TYPE_SPIFFS) ? SPIFFS_MOUNT_POINT : SD_MOUNT_POINT;
    char full_path[128];
    snprintf(full_path, sizeof(full_path), "%s%s", base_path, path);

    DIR *dir = opendir(full_path);
    if (!dir) {
        ESP_LOGE(TAG, "Failed to open directory %s", full_path);
        return ESP_FAIL;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_DIR) {
            // Add to directory list
            if (dir_list) {
                // Assuming dir_list is a pointer to a lv_obj_t (e.g., lv_list_t or lv_dropdown_t)
                // This part needs to be adapted based on how you want to store/display the list
                // For now, just print to log
                ESP_LOGI(TAG, "  DIR : %s", entry->d_name);
            }
        } else {
            // Add to file list
            if (file_list) {
                // Assuming file_list is a pointer to a lv_obj_t
                // For now, just print to log
                ESP_LOGI(TAG, "  FILE: %s", entry->d_name);
            }
        }
    }
    closedir(dir);
    return ESP_OK;
}

int storage_service_read_file(storage_type_t storage_type, const char *path, char *buffer, size_t max_len)
{
    const char *base_path = (storage_type == STORAGE_TYPE_SPIFFS) ? SPIFFS_MOUNT_POINT : SD_MOUNT_POINT;
    char full_path[128];
    snprintf(full_path, sizeof(full_path), "%s%s", base_path, path);

    FILE *f = fopen(full_path, "r");
    if (!f) {
        ESP_LOGE(TAG, "Failed to open file for reading %s", full_path);
        return -1;
    }

    int bytes_read = fread(buffer, 1, max_len, f);
    fclose(f);
    if (bytes_read < 0) {
        ESP_LOGE(TAG, "Error reading file %s", full_path);
        return -1;
    }
    buffer[bytes_read] = '\0'; // Null-terminate the string
    return bytes_read;
}

int storage_service_write_file(storage_type_t storage_type, const char *path, const char *content, size_t len)
{
    const char *base_path = (storage_type == STORAGE_TYPE_SPIFFS) ? SPIFFS_MOUNT_POINT : SD_MOUNT_POINT;
    char full_path[128];
    snprintf(full_path, sizeof(full_path), "%s%s", base_path, path);

    FILE *f = fopen(full_path, "w");
    if (!f) {
        ESP_LOGE(TAG, "Failed to open file for writing %s", full_path);
        return -1;
    }

    int bytes_written = fwrite(content, 1, len, f);
    fclose(f);
    if (bytes_written < 0) {
        ESP_LOGE(TAG, "Error writing file %s", full_path);
        return -1;
    }
    return bytes_written;
}


