/**
 * @file storage_manager.h
 * @brief Gestion du stockage sur carte SD via expander SPI
 */

#ifndef STORAGE_MANAGER_H
#define STORAGE_MANAGER_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initialisation du périphérique de stockage
 */
void storage_manager_init(void);

/**
 * @brief Lecture d'un fichier dans un buffer
 */
bool storage_manager_read(const char *path, uint8_t *buf, uint32_t len);

/**
 * @brief Écriture d'un buffer dans un fichier
 */
bool storage_manager_write(const char *path, const uint8_t *buf, uint32_t len);

/**
 * @brief Export des logs ou de la configuration
 */
bool storage_manager_export(const char *path);

/**
 * @brief Importe une configuration depuis la carte
 */
bool storage_manager_import(const char *path);

#ifdef __cplusplus
}
#endif

#endif /* STORAGE_MANAGER_H */
