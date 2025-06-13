/**
 * @file can_manager.c
 * @brief Implémentation du gestionnaire CAN
 */

#include "can_manager.h"
#include <stdio.h>

/** Configuration courante du bus */
static can_config_t s_config = {
    .bitrate = 500000,
    .termination = true
};

void can_manager_init(const can_config_t *config)
{
    if (config) {
        s_config = *config;
    }
    printf("[CAN] init bitrate=%lu termination=%d\n",
           (unsigned long)s_config.bitrate, s_config.termination);
    /* Ici on initialiserait le driver CAN de l'ESP-IDF */
}

void can_manager_set_bitrate(uint32_t bitrate)
{
    s_config.bitrate = bitrate;
    printf("[CAN] bitrate change -> %lu\n", (unsigned long)bitrate);
    /* Reconfiguration du périphérique */
}

void can_manager_enable_termination(bool enable)
{
    s_config.termination = enable;
    printf("[CAN] termination %s\n", enable ? "ON" : "OFF");
    /* Contrôle de la broche de terminaison si disponible */
}

void can_manager_log_frame(const can_frame_t *frame)
{
    if (!frame) {
        return;
    }
    printf("[CAN] id=0x%X dlc=%u", frame->id, frame->dlc);
    for (uint8_t i = 0; i < frame->dlc && i < 8; ++i) {
        printf(" %02X", frame->data[i]);
    }
    printf("\n");
}
