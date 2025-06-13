/**
 * @file rs485_manager.c
 * @brief Implémentation de l'abstraction RS485/Modbus RTU
 */

#include "rs485_manager.h"
#include <stdio.h>

/** Configuration courante */
static rs485_config_t s_cfg = {
    .uart_port = 1,
    .tx_en_gpio = -1,
    .baudrate = 9600
};

void rs485_manager_init(const rs485_config_t *config)
{
    if (config) {
        s_cfg = *config;
    }
    printf("[RS485] init uart=%d baud=%lu\n", s_cfg.uart_port,
           (unsigned long)s_cfg.baudrate);
    /* Initialisation de l'UART et de la broche TX_EN si nécessaire */
}

void rs485_manager_send(const uint8_t *pdu, uint16_t len)
{
    printf("[RS485] TX %u bytes\n", (unsigned)len);
    /* Ici on enverrait len octets sur la liaison UART en gérant la direction */
    (void)pdu;
}

void rs485_manager_poll_example(void)
{
    /* Exemple de polling fictif d'un esclave Modbus */
    uint8_t req[] = { 0x01, 0x03, 0x00, 0x00, 0x00, 0x02 }; /* sans CRC */
    rs485_manager_send(req, sizeof(req));
    /* Normalement attendre la réponse ici */
    printf("[RS485] polling example sent\n");
}

void rs485_manager_show_ui(void)
{
    /* Cette fonction serait reliée à l'interface LVGL pour afficher l'état RS485 */
    printf("[RS485] show UI\n");
}
