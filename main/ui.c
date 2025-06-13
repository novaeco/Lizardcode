/**
 * @file ui.c
 * @brief Implémentation du module ui
 */

#include "ui.h"
#include "lcd_manager.h"
#include "touch_manager.h"
#include "language_manager.h"
#include "diagnostics.h"
#include "battery_manager.h"
#include "wifi_manager.h"
#include "ble_manager.h"
#include "can_manager.h"
#include "rs485_manager.h"
#include "gpio_manager.h"
#include "storage_manager.h"
#include "esp_timer.h"
#include <stdio.h>

#define LVGL_TICK_PERIOD_MS 2

static ui_lang_t current_lang = UI_LANG_EN;
static ui_theme_t current_theme = UI_THEME_LIGHT;
static ui_readability_t current_readability = UI_READABILITY_NORMAL;

static lv_obj_t *cpu_label;
static lv_obj_t *ram_label;
static lv_obj_t *fps_label;
static lv_obj_t *rssi_label;
static lv_obj_t *battery_label;
static lv_obj_t *notification_label;
static lv_obj_t *keyboard;

static void lv_tick_task(void *arg);
static void display_flush(lv_disp_drv_t *drv, const lv_area_t *area, lv_color_t *color_p);
static void create_base_ui(void);
static void create_diagnostics_page(void);
static void apply_theme(void);
static const char *tr(const char *fr, const char *en);

static void lv_tick_task(void *arg)
{
    (void)arg;
    lv_tick_inc(LVGL_TICK_PERIOD_MS);
}

static void display_flush(lv_disp_drv_t *drv, const lv_area_t *area, lv_color_t *color_p)
{
    /* Fonction de flush simplifiée */
    (void)area;
    (void)color_p;
    lv_disp_flush_ready(drv);
}

static const char *tr(const char *fr, const char *en)
{
    return current_lang == UI_LANG_FR ? fr : en;
}

static void create_base_ui(void)
{
    lv_obj_t *scr = lv_scr_act();

    lv_obj_t *btn = lv_btn_create(scr);
    lv_obj_align(btn, LV_ALIGN_TOP_LEFT, 10, 10);
    lv_obj_t *lbl = lv_label_create(btn);
    lv_label_set_text(lbl, tr("Menu", "Menu"));

    lv_obj_t *slider = lv_slider_create(scr);
    lv_obj_set_width(slider, LV_PCT(80));
    lv_obj_align(slider, LV_ALIGN_TOP_MID, 0, 60);

    keyboard = lv_keyboard_create(scr);
    lv_obj_add_flag(keyboard, LV_OBJ_FLAG_HIDDEN);
}

static void create_diagnostics_page(void)
{
    lv_obj_t *cont = lv_obj_create(lv_scr_act());
    lv_obj_set_size(cont, LV_PCT(100), 100);
    lv_obj_align(cont, LV_ALIGN_BOTTOM_MID, 0, 0);

    cpu_label = lv_label_create(cont);
    lv_label_set_text(cpu_label, "CPU: 0%");
    lv_obj_align(cpu_label, LV_ALIGN_TOP_LEFT, 10, 10);

    ram_label = lv_label_create(cont);
    lv_label_set_text(ram_label, "RAM: 0KB");
    lv_obj_align(ram_label, LV_ALIGN_TOP_LEFT, 10, 30);

    fps_label = lv_label_create(cont);
    lv_label_set_text(fps_label, "FPS: 0");
    lv_obj_align(fps_label, LV_ALIGN_TOP_LEFT, 10, 50);

    rssi_label = lv_label_create(cont);
    lv_label_set_text(rssi_label, "RSSI: 0dBm");
    lv_obj_align(rssi_label, LV_ALIGN_TOP_LEFT, 150, 10);

    battery_label = lv_label_create(cont);
    lv_label_set_text(battery_label, "BAT: 0%");
    lv_obj_align(battery_label, LV_ALIGN_TOP_LEFT, 150, 30);
}

static void apply_theme(void)
{
    lv_disp_t *disp = lv_disp_get_default();
    lv_theme_t *theme = lv_theme_default_init(disp,
            current_theme == UI_THEME_DARK ? lv_color_black() : lv_color_white(),
            lv_palette_main(LV_PALETTE_BLUE),
            current_theme == UI_THEME_DARK,
            LV_FONT_DEFAULT);
    lv_disp_set_theme(disp, theme);
}

void ui_init(void)
{
    lcd_manager_init();
    touch_manager_init();

    lv_init();

    static lv_disp_draw_buf_t draw_buf;
    static lv_color_t buf1[LV_HOR_RES_MAX * 40];
    lv_disp_draw_buf_init(&draw_buf, buf1, NULL, LV_HOR_RES_MAX * 40);

    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.draw_buf = &draw_buf;
    disp_drv.hor_res = LV_HOR_RES_MAX;
    disp_drv.ver_res = LV_VER_RES_MAX;
    disp_drv.flush_cb = display_flush;
    lv_disp_drv_register(&disp_drv);

    const esp_timer_create_args_t tick_args = {
        .callback = &lv_tick_task,
        .name = "lvgl"
    };
    esp_timer_handle_t tick_timer;
    esp_timer_create(&tick_args, &tick_timer);
    esp_timer_start_periodic(tick_timer, LVGL_TICK_PERIOD_MS * 1000);

    apply_theme();
    create_base_ui();
    create_diagnostics_page();
}

void ui_set_language(ui_lang_t lang)
{
    current_lang = lang;
}

void ui_set_theme(ui_theme_t theme)
{
    current_theme = theme;
    apply_theme();
}

void ui_set_readability(ui_readability_t level)
{
    current_readability = level;
    (void)current_readability;
}

void ui_show_notification(const char *text)
{
    if (!notification_label) {
        notification_label = lv_label_create(lv_scr_act());
        lv_obj_align(notification_label, LV_ALIGN_TOP_MID, 0, 100);
    }
    lv_label_set_text(notification_label, text);
}

void ui_update_diagnostics(void)
{
    char buf[32];
    snprintf(buf, sizeof(buf), "CPU: %d%%", 0); /* valeur fictive */
    lv_label_set_text(cpu_label, buf);

    snprintf(buf, sizeof(buf), "RAM: %dKB", 0);
    lv_label_set_text(ram_label, buf);

    snprintf(buf, sizeof(buf), "FPS: %d", 0);
    lv_label_set_text(fps_label, buf);

    snprintf(buf, sizeof(buf), "RSSI: %d", 0);
    lv_label_set_text(rssi_label, buf);

    snprintf(buf, sizeof(buf), "BAT: %d%%", 0);
    lv_label_set_text(battery_label, buf);
}
