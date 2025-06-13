/**
 * @file gpio_manager.c
 * @brief Implémentation du gestionnaire GPIO
 */

#include "gpio_manager.h"
#include <stdio.h>

/** Association des GPIO logiques vers les broches physiques */
static int s_gpio_map[GPIO_MANAGER_MAX];

void gpio_manager_init(void)
{
    for (int i = 0; i < GPIO_MANAGER_MAX; ++i) {
        s_gpio_map[i] = -1; /* -1 signifie non assigné */
    }
    printf("[GPIO] init\n");
}

bool gpio_manager_read(uint8_t index)
{
    if (index >= GPIO_MANAGER_MAX || s_gpio_map[index] < 0) {
        return false;
    }
    /* Lecture réelle du GPIO ici */
    printf("[GPIO] read index=%u gpio=%d\n", index, s_gpio_map[index]);
    return false;
}

void gpio_manager_write(uint8_t index, bool level)
{
    if (index >= GPIO_MANAGER_MAX || s_gpio_map[index] < 0) {
        return;
    }
    /* Écriture réelle du GPIO ici */
    printf("[GPIO] write index=%u gpio=%d level=%d\n",
           index, s_gpio_map[index], level);
}

void gpio_manager_remap(uint8_t index, int gpio_num)
{
    if (index >= GPIO_MANAGER_MAX) {
        return;
    }
    s_gpio_map[index] = gpio_num;
    printf("[GPIO] remap index=%u -> gpio %d\n", index, gpio_num);
}

void gpio_manager_show_remap_menu(void)
{
    /* Cette fonction afficherait un menu dans la GUI pour choisir les mappings */
    printf("[GPIO] show remap menu\n");
}
