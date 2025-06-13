/**
 * @file rtc_manager.c
 * @brief Implémentation de la gestion RTC
 */

#include "rtc_manager.h"
#include <stdio.h>

/** Type de RTC détecté */
typedef enum {
    RTC_NONE,
    RTC_DS1307,
    RTC_PCF8563
} rtc_type_t;

static rtc_type_t s_type = RTC_NONE;
static rtc_time_t s_time; /* valeur simulée */

static void autodetect_rtc(void)
{
    /* Routine simplifiée de détection sur le bus I2C 0x68/0x51 */
    printf("[RTC] autodetect...\n");
    s_type = RTC_DS1307; /* valeur fictive */
}

void rtc_manager_init(void)
{
    autodetect_rtc();
    printf("[RTC] type=%d\n", s_type);
}

bool rtc_manager_get_time(rtc_time_t *t)
{
    if (!t) {
        return false;
    }
    /* Lecture du RTC réel ici */
    *t = s_time;
    return true;
}

bool rtc_manager_set_time(const rtc_time_t *t)
{
    if (!t) {
        return false;
    }
    /* Écriture du RTC réel ici */
    s_time = *t;
    printf("[RTC] time set %04u-%02u-%02u %02u:%02u:%02u\n",
           t->year, t->month, t->day, t->hour, t->min, t->sec);
    return true;
}
