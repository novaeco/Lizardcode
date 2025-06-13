/**
 * @file lcd_manager.h
 * @brief Module lcd_manager
 */

#ifndef LCD_MANAGER_H
#define LCD_MANAGER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

/**
 * @brief Modèles d'écran gérés
 */
typedef enum {
    LCD_MODEL_UNKNOWN = 0,      /**< Modèle non détecté */
    LCD_MODEL_WAVESHARE_7INCH,  /**< Ecran Waveshare 7" 800x480 */
    LCD_MODEL_WAVESHARE_5B      /**< Ecran Waveshare 5B 1024x600 */
} lcd_model_t;

/**
 * @brief Configuration détectée de l'écran
 */
typedef struct {
    lcd_model_t model;  /**< Type de dalle LCD */
    uint16_t    width;  /**< Largeur en pixels */
    uint16_t    height; /**< Hauteur en pixels */
} lcd_config_t;

/** I2C utilisé pour la détection LCD/tactile */
#define LCD_MANAGER_I2C_PORT     I2C_NUM_0
#define LCD_MANAGER_I2C_SDA      18
#define LCD_MANAGER_I2C_SCL      17
#define LCD_MANAGER_I2C_FREQ_HZ  400000

/**
 * @brief Initialise la détection et la configuration de l'écran
 */
void lcd_manager_init(void);

/**
 * @brief Récupère la configuration détectée
 *
 * @return Pointeur vers la structure de configuration (valide après init)
 */
const lcd_config_t *lcd_manager_get_config(void);

#ifdef __cplusplus
}
#endif

#endif /* LCD_MANAGER_H */
