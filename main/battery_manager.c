/**
 * @file battery_manager.c
 * @brief Implémentation du suivi batterie
 */

#include "battery_manager.h"
#include <stdio.h>

static int s_adc_channel = -1;
static int s_gpio_test = -1;
static uint8_t s_level = 100; /* valeur simulée */

void battery_manager_init(int adc_channel, int gpio_test)
{
    s_adc_channel = adc_channel;
    s_gpio_test = gpio_test;
    printf("[BAT] init adc=%d gpio_test=%d\n", s_adc_channel, s_gpio_test);
    /* Configuration de l'ADC et du GPIO ici */
}

uint8_t battery_manager_get_level(void)
{
    /* Mesure réelle sur l'ADC ici */
    printf("[BAT] level=%u%%\n", s_level);
    return s_level;
}

bool battery_manager_gpio_test(void)
{
    /* Lecture du GPIO de test */
    printf("[BAT] gpio test on %d\n", s_gpio_test);
    return true; /* valeur fictive */
}
