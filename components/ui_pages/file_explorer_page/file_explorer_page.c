#include "file_explorer_page.h"
#include "lvgl.h"
#include "esp_log.h"
#include "storage_service.h"
#include "i18n_service.h"
#include <string.h>

static const char *TAG = "FILE_EXPLORER_PAGE";
static lv_obj_t *file_list_obj;
static lv_obj_t *current_path_label;
static storage_type_t current_storage_type = STORAGE_TYPE_SPIFFS;

static void parent_dir_event_cb(lv_event_t *e);

static void update_file_list(void)
{
    lv_obj_clean(file_list_obj); // Clear existing list items

    char current_path[128];
    lv_label_get_text(current_path_label, current_path, sizeof(current_path));

    // Dummy lists for now, will be populated by storage_service_list_dir
    // In a real implementation, you'd have dynamic lists or arrays to store results
    // and then add them to LVGL list/table.
    lv_obj_t *item;

    // Example: Add parent directory option
    if (strcmp(current_path, "/") != 0) {
        item = lv_list_add_btn(file_list_obj, LV_SYMBOL_UP, "..");
        lv_obj_add_event_cb(item, parent_dir_event_cb, LV_EVENT_CLICKED, NULL);
    }

    // Placeholder for actual file/directory listing
    // You would call storage_service_list_dir here and iterate through results
    // For demonstration, let's add some dummy entries
    if (current_storage_type == STORAGE_TYPE_SPIFFS) {
        lv_list_add_btn(file_list_obj, LV_SYMBOL_DIRECTORY, "folder1");
        lv_list_add_btn(file_list_obj, LV_SYMBOL_FILE, "file1.txt");
    } else {
        lv_list_add_btn(file_list_obj, LV_SYMBOL_DIRECTORY, "sd_folder");
        lv_list_add_btn(file_list_obj, LV_SYMBOL_FILE, "sd_image.jpg");
    }

    // Example of how to use storage_service_list_dir (requires dynamic list handling)
    // esp_err_t ret = storage_service_list_dir(current_storage_type, current_path, NULL, NULL);
    // if (ret != ESP_OK) {
    //     ESP_LOGE(TAG, "Failed to list directory: %s", esp_err_to_name(ret));
    // }
}

static void parent_dir_event_cb(lv_event_t *e)
{
    char path[128];
    lv_label_get_text(current_path_label, path, sizeof(path));
    char *last_slash = strrchr(path, '/');
    if (last_slash && last_slash != path) {
        *last_slash = '\0';
    } else {
        strcpy(path, "/");
    }
    lv_label_set_text(current_path_label, path);
    update_file_list();
}

static void spiffs_btn_event_cb(lv_event_t *e)
{
    current_storage_type = STORAGE_TYPE_SPIFFS;
    lv_label_set_text(current_path_label, "/");
    update_file_list();
    ESP_LOGI(TAG, "Switched to SPIFFS.");
}

static void sd_btn_event_cb(lv_event_t *e)
{
    current_storage_type = STORAGE_TYPE_SD_CARD;
    lv_label_set_text(current_path_label, "/");
    update_file_list();
    ESP_LOGI(TAG, "Switched to SD Card.");
}

void create_file_explorer_page(lv_obj_t *parent)
{
    lv_obj_t *file_explorer_screen = lv_obj_create(parent);
    lv_obj_set_size(file_explorer_screen, LV_HOR_RES, LV_VER_RES);
    lv_obj_set_flex_flow(file_explorer_screen, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(file_explorer_screen, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_START);
    lv_obj_set_style_pad_all(file_explorer_screen, 10, 0);

    lv_obj_t *title_label = lv_label_create(file_explorer_screen);
    lv_label_set_text(title_label, i18n_service_get_string("file_explorer_page_title"));
    lv_obj_set_style_text_font(title_label, &lv_font_montserrat_28, 0);
    lv_obj_set_style_align(title_label, LV_ALIGN_CENTER, 0);

    lv_obj_t *storage_type_cont = lv_obj_create(file_explorer_screen);
    lv_obj_set_size(storage_type_cont, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_set_flex_flow(storage_type_cont, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(storage_type_cont, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_bg_opa(storage_type_cont, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(storage_type_cont, 0, 0);
    lv_obj_set_style_pad_all(storage_type_cont, 0, 0);

    lv_obj_t *spiffs_btn = lv_btn_create(storage_type_cont);
    lv_obj_set_size(spiffs_btn, 120, 40);
    lv_obj_t *spiffs_label = lv_label_create(spiffs_btn);
    lv_label_set_text(spiffs_label, i18n_service_get_string("browse_spiffs_btn"));
    lv_obj_center(spiffs_label);
    lv_obj_add_event_cb(spiffs_btn, spiffs_btn_event_cb, LV_EVENT_CLICKED, NULL);

    lv_obj_t *sd_btn = lv_btn_create(storage_type_cont);
    lv_obj_set_size(sd_btn, 120, 40);
    lv_obj_t *sd_label = lv_label_create(sd_btn);
    lv_label_set_text(sd_label, i18n_service_get_string("browse_sd_btn"));
    lv_obj_center(sd_label);
    lv_obj_add_event_cb(sd_btn, sd_btn_event_cb, LV_EVENT_CLICKED, NULL);

    current_path_label = lv_label_create(file_explorer_screen);
    lv_label_set_text(current_path_label, "/");
    lv_obj_set_style_text_font(current_path_label, &lv_font_montserrat_20, 0);
    lv_obj_set_style_align(current_path_label, LV_ALIGN_LEFT, 0);

    file_list_obj = lv_list_create(file_explorer_screen);
    lv_obj_set_size(file_list_obj, LV_HOR_RES - 20, LV_VER_RES - 150);
    lv_obj_set_style_bg_color(file_list_obj, lv_color_hex(0x222222), 0);
    lv_obj_set_style_border_width(file_list_obj, 0, 0);
    lv_obj_set_style_pad_all(file_list_obj, 5, 0);

    update_file_list(); // Initial list population

    ESP_LOGI(TAG, "File explorer page created.");
}


