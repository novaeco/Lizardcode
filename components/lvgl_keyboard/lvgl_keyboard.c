#include "lvgl_keyboard.h"
#include "esp_log.h"

static const char *TAG = "LVGL_KEYBOARD";

static lv_obj_t *keyboard;
static lv_obj_t *target_textarea;

static void textarea_event_cb(lv_event_t *e);

// AZERTY keyboard keymap for lowercase
static const char *kb_map_azerty_lc[] = {
    "a", "z", "e", "r", "t", "y", "u", "i", "o", "p", "\n",
    "q", "s", "d", "f", "g", "h", "j", "k", "l", "m", "\n",
    "w", "x", "c", "v", "b", "n", ",", ".", ";", ":", "\n",
    LV_SYMBOL_UP, LV_SYMBOL_KEYBOARD, " ", LV_SYMBOL_BACKSPACE, LV_SYMBOL_OK, ""
};

// AZERTY keyboard keymap for uppercase
static const char *kb_map_azerty_uc[] = {
    "A", "Z", "E", "R", "T", "Y", "U", "I", "O", "P", "\n",
    "Q", "S", "D", "F", "G", "H", "J", "K", "L", "M", "\n",
    "W", "X", "C", "V", "B", "N", "?", "!", "/", "(", ")", "\n",
    LV_SYMBOL_DOWN, LV_SYMBOL_KEYBOARD, " ", LV_SYMBOL_BACKSPACE, LV_SYMBOL_OK, ""
};

// AZERTY keyboard keymap for special characters (accents and symbols)
static const char *kb_map_azerty_spec[] = {
    "&", "é", "\"", "\'', "(", "-", "è", "_", "ç", "à", "\n",
    ")", "=", "+", "{", "}", "[", "]", "@", "#", "~", "\n",
    "<", ">", "|", "\\", "/", "*", "€", "£", "$", "°", "\n",
    LV_SYMBOL_LEFT, LV_SYMBOL_KEYBOARD, " ", LV_SYMBOL_RIGHT, LV_SYMBOL_BACKSPACE, LV_SYMBOL_OK, ""
};

static void keyboard_event_cb(lv_event_t *e)
{
    lv_obj_t *obj = lv_event_get_target(e);
    lv_event_code_t code = lv_event_get_code(e);

    if (code == LV_EVENT_VALUE_CHANGED) {
        uint32_t btn_id = lv_btnmatrix_get_selected_btn(obj);
        if (btn_id == LV_BUTTON_MATRIX_BTN_NONE) return;

        const char *txt = lv_btnmatrix_get_btn_text(obj, btn_id);

        if (strcmp(txt, LV_SYMBOL_UP) == 0) {
            lv_btnmatrix_set_map(obj, kb_map_azerty_uc);
        } else if (strcmp(txt, LV_SYMBOL_DOWN) == 0) {
            lv_btnmatrix_set_map(obj, kb_map_azerty_lc);
        } else if (strcmp(txt, LV_SYMBOL_KEYBOARD) == 0) {
            // Toggle between lowercase, uppercase, and special characters
            if (lv_btnmatrix_get_map(obj) == kb_map_azerty_lc) {
                lv_btnmatrix_set_map(obj, kb_map_azerty_uc);
            } else if (lv_btnmatrix_get_map(obj) == kb_map_azerty_uc) {
                lv_btnmatrix_set_map(obj, kb_map_azerty_spec);
            } else {
                lv_btnmatrix_set_map(obj, kb_map_azerty_lc);
            }
        } else if (strcmp(txt, LV_SYMBOL_BACKSPACE) == 0) {
            if (target_textarea) lv_textarea_del_char(target_textarea);
        } else if (strcmp(txt, LV_SYMBOL_OK) == 0) {
            if (target_textarea) lv_event_send(target_textarea, LV_EVENT_READY, NULL);
            lv_obj_add_flag(keyboard, LV_OBJ_FLAG_HIDDEN); // Hide keyboard on OK
        } else {
            if (target_textarea) lv_textarea_add_text(target_textarea, txt);
        }
    }
}

lv_obj_t *lvgl_keyboard_create(lv_obj_t *parent)
{
    keyboard = lv_keyboard_create(parent);
    lv_obj_set_size(keyboard, LV_HOR_RES, LV_VER_RES / 2); // Adjust size as needed
    lv_obj_align(keyboard, LV_ALIGN_BOTTOM_MID, 0, 0);
    lv_keyboard_set_map(keyboard, LV_KEYBOARD_MODE_USER, kb_map_azerty_lc); // Set initial keymap to lowercase
    lv_obj_add_event_cb(keyboard, keyboard_event_cb, LV_EVENT_ALL, NULL);
    lv_obj_add_flag(keyboard, LV_OBJ_FLAG_HIDDEN); // Hidden by default

    ESP_LOGI(TAG, "AZERTY keyboard created.");
    return keyboard;
}

static void textarea_event_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if (code == LV_EVENT_DEFOCUSED) {
        lv_obj_add_flag(keyboard, LV_OBJ_FLAG_HIDDEN);
    }
}

void lvgl_keyboard_set_textarea(lv_obj_t *kb, lv_obj_t *ta)
{
    keyboard = kb;
    target_textarea = ta;
    lv_keyboard_set_textarea(keyboard, target_textarea);
    lv_obj_clear_flag(keyboard, LV_OBJ_FLAG_HIDDEN); // Show keyboard when setting textarea

    // Add event handler to hide keyboard when textarea is unfocused
    lv_obj_add_event_cb(target_textarea, textarea_event_cb, LV_EVENT_ALL, NULL);
}


