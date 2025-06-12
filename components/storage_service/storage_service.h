#ifndef STORAGE_SERVICE_H
#define STORAGE_SERVICE_H

#include "esp_err.h"
#include <stdbool.h>
#include <dirent.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    STORAGE_TYPE_SPIFFS,
    STORAGE_TYPE_SD_CARD
} storage_type_t;

/**
 * @brief Initialize the storage service (SPIFFS and SD card).
 * @return ESP_OK on success, otherwise an error code.
 */
esp_err_t storage_service_init(void);

/**
 * @brief List files and directories in a given path.
 * @param storage_type Type of storage (SPIFFS or SD_CARD).
 * @param path The path to list.
 * @param file_list A pointer to a linked list or array to store the file names.
 * @param dir_list A pointer to a linked list or array to store the directory names.
 * @return ESP_OK on success, otherwise an error code.
 */
esp_err_t storage_service_list_dir(storage_type_t storage_type, const char *path, void *file_list, void *dir_list);

/**
 * @brief Read content of a file.
 * @param storage_type Type of storage (SPIFFS or SD_CARD).
 * @param path The path to the file.
 * @param buffer Buffer to store the file content.
 * @param max_len Maximum length to read.
 * @return Number of bytes read, or -1 on error.
 */
int storage_service_read_file(storage_type_t storage_type, const char *path, char *buffer, size_t max_len);

/**
 * @brief Write content to a file.
 * @param storage_type Type of storage (SPIFFS or SD_CARD).
 * @param path The path to the file.
 * @param content Content to write.
 * @param len Length of content.
 * @return Number of bytes written, or -1 on error.
 */
int storage_service_write_file(storage_type_t storage_type, const char *path, const char *content, size_t len);

#ifdef __cplusplus
}
#endif

#endif // STORAGE_SERVICE_H


