/**
 * @file rs485_manager.h
 * @brief Abstraction Modbus RTU sur liaison RS485
 */

#ifndef RS485_MANAGER_H
#define RS485_MANAGER_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/** Configuration de la couche RS485 */
typedef struct {
    int uart_port;            /**< Port UART à utiliser */
    int tx_en_gpio;           /**< GPIO de contrôle direction driver */
    uint32_t baudrate;        /**< Vitesse de communication */
} rs485_config_t;

/**
 * @brief Initialisation du bus RS485
 */
void rs485_manager_init(const rs485_config_t *config);

/**
 * @brief Envoi d'une trame Modbus RTU
 */
void rs485_manager_send(const uint8_t *pdu, uint16_t len);

/**
 * @brief Fonction de polling d'exemple (lecture/esclave)
 */
void rs485_manager_poll_example(void);

/**
 * @brief Affichage dans l'interface utilisateur
 */
void rs485_manager_show_ui(void);

#ifdef __cplusplus
}
#endif

#endif /* RS485_MANAGER_H */
