#include "demo_ui.h"

#include <stdint.h>

#include "lvgl.h"
#include "modbus_port.h"

#define UI_REFRESH_MS 200
#define CARD_SIZE     180

static lv_obj_t *s_value_labels[MODBUS_HOLDING_REG_COUNT];
static const uint32_t s_card_colors[MODBUS_HOLDING_REG_COUNT] = {
    0x1565C0, 0x2E7D32, 0xEF6C00, 0x6A1B9A,
};

typedef struct {
    size_t index;
    int8_t delta;
} value_button_context_t;

static value_button_context_t s_button_contexts[MODBUS_HOLDING_REG_COUNT][2];

static void value_button_cb(lv_event_t *event)
{
    const value_button_context_t *context = lv_event_get_user_data(event);
    uint16_t value = modbus_port_get_holding(context->index);

    if (context->delta > 0 && value < UINT16_MAX) {
        value++;
    } else if (context->delta < 0 && value > 0) {
        value--;
    }
    (void)modbus_port_set_holding(context->index, value);
}

static void refresh_cb(lv_timer_t *timer)
{
    (void)timer;

    for (size_t i = 0; i < MODBUS_HOLDING_REG_COUNT; ++i) {
        lv_label_set_text_fmt(s_value_labels[i], "%u",
                              (unsigned)modbus_port_get_holding(i));
    }
}

void demo_ui_create(void)
{
    static const lv_point_t positions[MODBUS_HOLDING_REG_COUNT] = {
        {45, 65}, {255, 65}, {45, 275}, {255, 275},
    };

    lv_obj_t *screen = lv_screen_active();
    lv_obj_clean(screen);
    lv_obj_set_style_bg_color(screen, lv_color_hex(0xECEFF1), 0);
    lv_obj_clear_flag(screen, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_t *title = lv_label_create(screen);
    lv_label_set_text(title, "RS485 VALUES  |  TOUCH TEST");
    lv_obj_set_style_text_font(title, &lv_font_montserrat_16, 0);
    lv_obj_set_style_text_color(title, lv_color_hex(0x263238), 0);
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 8);

    lv_obj_t *info = lv_label_create(screen);
    lv_label_set_text_fmt(info, "ID 1   115200 8N1   %s",
                          modbus_port_is_running() ? "READY" : "ERROR");
    lv_obj_set_style_text_font(info, &lv_font_montserrat_14, 0);
    lv_obj_set_style_text_color(info,
                                lv_color_hex(modbus_port_is_running() ? 0x2E7D32 : 0xC62828), 0);
    lv_obj_align(info, LV_ALIGN_TOP_MID, 0, 30);

    for (size_t i = 0; i < MODBUS_HOLDING_REG_COUNT; ++i) {
        lv_obj_t *card = lv_obj_create(screen);
        lv_obj_set_size(card, CARD_SIZE, CARD_SIZE);
        lv_obj_set_pos(card, positions[i].x, positions[i].y);
        lv_obj_set_style_radius(card, 18, 0);
        lv_obj_set_style_border_width(card, 0, 0);
        lv_obj_set_style_bg_color(card, lv_color_hex(s_card_colors[i]), 0);
        lv_obj_set_style_shadow_width(card, 18, 0);
        lv_obj_set_style_shadow_opa(card, LV_OPA_30, 0);
        lv_obj_clear_flag(card, LV_OBJ_FLAG_SCROLLABLE);

        lv_obj_t *address_label = lv_label_create(card);
        lv_label_set_text_fmt(address_label, "4%04u", (unsigned)(i + 1));
        lv_obj_set_style_text_font(address_label, &lv_font_montserrat_16, 0);
        lv_obj_set_style_text_color(address_label, lv_color_hex(0xFFFFFF), 0);
        lv_obj_align(address_label, LV_ALIGN_TOP_MID, 0, 5);

        s_value_labels[i] = lv_label_create(card);
        lv_label_set_text_fmt(s_value_labels[i], "%u",
                              (unsigned)modbus_port_get_holding(i));
        lv_obj_set_style_text_align(s_value_labels[i], LV_TEXT_ALIGN_CENTER, 0);
        lv_obj_set_style_text_font(s_value_labels[i], &lv_font_montserrat_22, 0);
        lv_obj_set_style_text_color(s_value_labels[i], lv_color_hex(0xFFFFFF), 0);
        lv_obj_align(s_value_labels[i], LV_ALIGN_CENTER, 0, -12);

        static const char *button_text[] = {"-1", "+1"};
        for (size_t button_index = 0; button_index < 2; ++button_index) {
            s_button_contexts[i][button_index] = (value_button_context_t) {
                .index = i,
                .delta = button_index == 0 ? -1 : 1,
            };

            lv_obj_t *button = lv_button_create(card);
            lv_obj_set_size(button, 68, 46);
            lv_obj_align(button,
                         button_index == 0 ? LV_ALIGN_BOTTOM_LEFT : LV_ALIGN_BOTTOM_RIGHT,
                         button_index == 0 ? 4 : -4, -4);
            lv_obj_set_style_bg_color(button, lv_color_hex(0xFFFFFF), 0);
            lv_obj_set_style_bg_color(button, lv_color_hex(0xCFD8DC), LV_STATE_PRESSED);
            lv_obj_add_event_cb(button, value_button_cb, LV_EVENT_CLICKED,
                                &s_button_contexts[i][button_index]);

            lv_obj_t *button_label = lv_label_create(button);
            lv_label_set_text(button_label, button_text[button_index]);
            lv_obj_set_style_text_font(button_label, &lv_font_montserrat_22, 0);
            lv_obj_set_style_text_color(button_label, lv_color_hex(s_card_colors[i]), 0);
            lv_obj_center(button_label);
        }
    }

    lv_timer_create(refresh_cb, UI_REFRESH_MS, NULL);
}
