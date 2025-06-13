/**
 * @file expander_ch422g.h
 * @brief Module expander_ch422g
 */

#ifndef EXPANDER_CH422G_H
#define EXPANDER_CH422G_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include "esp_err.h"

/**
 * @brief Structure de gestion du composant CH422G
 */
typedef struct {
    bool       present;   /**< Vrai si l'extension est détectée */
    uint8_t    address;   /**< Adresse I2C utilisée pour les registres */
    int        port;      /**< Port I2C associé */
    uint8_t    wr_set;    /**< Copie locale du registre WR_SET */
    uint8_t    wr_oc;     /**< Copie locale du registre WR_OC */
    uint8_t    wr_io;     /**< Copie locale du registre WR_IO */
} expander_ch422g_t;

/**
 * @brief Initialise l'extension et détecte sa présence
 *
 * @param dev  Structure de périphérique à initialiser
 * @param port Port I2C à utiliser
 * @return true si l'extension est détectée
 */
bool expander_ch422g_init(expander_ch422g_t *dev, int port);

/**
 * @brief Configure tous les ports de l'extension en sortie push-pull
 */
esp_err_t expander_ch422g_set_all_output(expander_ch422g_t *dev);

/**
 * @brief Écrit un mot de 16 bits sur les sorties
 */
esp_err_t expander_ch422g_write(expander_ch422g_t *dev, uint16_t value);

/**
 * @brief Lit l'état des entrées
 */
esp_err_t expander_ch422g_read(expander_ch422g_t *dev, uint8_t *value);

#ifdef __cplusplus
}
#endif

#endif /* EXPANDER_CH422G_H */
