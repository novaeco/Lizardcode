/**
 * @file rtc_manager.h
 * @brief Gestion de l'horloge temps réel externe
 */

#ifndef RTC_MANAGER_H
#define RTC_MANAGER_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Informations temporelles basiques
 */
typedef struct {
    uint16_t year;  /**< Année */
    uint8_t  month; /**< Mois (1-12) */
    uint8_t  day;   /**< Jour (1-31) */
    uint8_t  hour;  /**< Heure */
    uint8_t  min;   /**< Minute */
    uint8_t  sec;   /**< Seconde */
} rtc_time_t;

/**
 * @brief Initialisation avec détection automatique du circuit RTC
 */
void rtc_manager_init(void);

/**
 * @brief Lecture de la date/heure courante
 */
bool rtc_manager_get_time(rtc_time_t *t);

/**
 * @brief Réglage manuel de la date/heure
 */
bool rtc_manager_set_time(const rtc_time_t *t);

#ifdef __cplusplus
}
#endif

#endif /* RTC_MANAGER_H */
