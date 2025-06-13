/**
 * @file storage_manager.c
 * @brief Implémentation de la gestion du stockage SD
 */

#include "storage_manager.h"
#include <stdio.h>

void storage_manager_init(void)
{
    /* Initialisation du bus SPI via l'expander si nécessaire */
    printf("[STORAGE] init\n");
}

bool storage_manager_read(const char *path, uint8_t *buf, uint32_t len)
{
    printf("[STORAGE] read %s len=%lu\n", path, (unsigned long)len);
    /* Ouverture et lecture du fichier via FATFS */
    (void)buf;
    return true;
}

bool storage_manager_write(const char *path, const uint8_t *buf, uint32_t len)
{
    printf("[STORAGE] write %s len=%lu\n", path, (unsigned long)len);
    (void)buf;
    return true;
}

bool storage_manager_export(const char *path)
{
    printf("[STORAGE] export -> %s\n", path);
    return true;
}

bool storage_manager_import(const char *path)
{
    printf("[STORAGE] import <- %s\n", path);
    return true;
}
