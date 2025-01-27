/* SPDX-License-Identifier: GPL-2.0-or-later */

#include QMK_KEYBOARD_H
#include "qp.h"
#include "utils/keycode_string.h"

#define _LY0 0
#define _LY1 1
#define _LY2 2
#define _LY3 3
#define _LY4 4
#define _LY5 5
#define _LY6 6
#define _LY7 7
#define _LY8 8
#define _RGB 9

/* LCD */
painter_device_t lcd;
static uint32_t type_count = 0 ;

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_LY0] = LAYOUT(
        KC_NO,     KC_F13,  KC_F14,  KC_NO,
        KC_F15,    KC_F16,  KC_F17,  KC_F16,
        KC_F18,    KC_F19,  KC_F20,  KC_F21,
        KC_NO,     KC_NO,   KC_NO,   KC_NO,
        KC_MS_U,   KC_MS_D, KC_MS_L, KC_MS_R,
        KC_MS_BTN1,TO(_LY1),TO(_LY8),KC_NO
    ),
    [_LY1] = LAYOUT(
        KC_NO,   KC_NO,   KC_NO,   KC_NO,
        KC_NO,   KC_NO,   KC_NO,   KC_NO,
        KC_NO,   KC_NO,   KC_NO,   KC_NO,
        KC_NO,   KC_NO,   KC_NO,   KC_NO,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, TO(_LY2),TO(_LY0),KC_NO
    ),
    [_LY2] = LAYOUT(
        KC_NO,   KC_NO,   KC_NO,   KC_NO,
        KC_NO,   KC_NO,   KC_NO,   KC_NO,
        KC_NO,   KC_NO,   KC_NO,   KC_NO,
        KC_NO,   KC_NO,   KC_NO,   KC_NO,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, TO(_LY3),TO(_LY1),KC_NO
    ),
    [_LY3] = LAYOUT(
        KC_NO,   KC_NO,   KC_NO,   KC_NO,
        KC_NO,   KC_NO,   KC_NO,   KC_NO,
        KC_NO,   KC_NO,   KC_NO,   KC_NO,
        KC_NO,   KC_NO,   KC_NO,   KC_NO,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, TO(_LY4),TO(_LY2),KC_NO
    ),
    [_LY4] = LAYOUT(
        KC_NO,   KC_NO,   KC_NO,   KC_NO,
        KC_NO,   KC_NO,   KC_NO,   KC_NO,
        KC_NO,   KC_NO,   KC_NO,   KC_NO,
        KC_NO,   KC_NO,   KC_NO,   KC_NO,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, TO(_LY5),TO(_LY3),KC_NO
    ),
    [_LY5] = LAYOUT(
        KC_NO,   KC_NO,   KC_NO,   KC_NO,
        KC_NO,   KC_NO,   KC_NO,   KC_NO,
        KC_NO,   KC_NO,   KC_NO,   KC_NO,
        KC_NO,   KC_NO,   KC_NO,   KC_NO,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, TO(_LY6),TO(_LY4),KC_NO
    ),
    [_LY6] = LAYOUT(
        KC_NO,   KC_NO,   KC_NO,   KC_NO,
        KC_NO,   KC_NO,   KC_NO,   KC_NO,
        KC_NO,   KC_NO,   KC_NO,   KC_NO,
        KC_NO,   KC_NO,   KC_NO,   KC_NO,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, TO(_LY7),TO(_LY5),KC_NO
    ),
    [_LY7] = LAYOUT(
        KC_NO,   KC_NO,   KC_NO,   KC_NO,
        KC_NO,   KC_NO,   KC_NO,   KC_NO,
        KC_NO,   KC_NO,   KC_NO,   KC_NO,
        KC_NO,   KC_NO,   KC_NO,   KC_NO,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, TO(_LY8),TO(_LY6),KC_NO
    ),
    [_LY8] = LAYOUT(
        KC_NO,   KC_NO,   KC_NO,   KC_NO,
        KC_NO,   KC_NO,   KC_NO,   KC_NO,
        KC_NO,   KC_NO,   KC_NO,   KC_NO,
        KC_NO,   KC_NO,   KC_NO,   KC_NO,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, TO(_LY0),TO(_LY7),KC_NO
    ),
    [_RGB] = LAYOUT(
        KC_NO,   KC_NO,   KC_NO,   KC_NO,
        KC_NO,   KC_NO,   KC_NO,   KC_NO,
        KC_NO,   KC_NO,   KC_NO,   KC_NO,
        KC_NO,   KC_NO,   KC_NO,   KC_NO,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_NO,   KC_NO,   KC_NO
    )
};

#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][2] = {
    //                  Encoder 1                         Encoder 2                         Encoder 3
    //                  Encoder 4                         Encoder 5                         Encoder 6
    [_LY0] =   { ENCODER_CCW_CW(KC_MPRV, KC_MNXT), ENCODER_CCW_CW(KC_VOLD, KC_VOLU), ENCODER_CCW_CW(KC_BRID, KC_BRIU),
                 ENCODER_CCW_CW(KC_WBAK, KC_WFWD), ENCODER_CCW_CW(KC_NO, KC_NO),     ENCODER_CCW_CW(KC_WH_U, KC_WH_D)},

    [_LY1] =   { ENCODER_CCW_CW(KC_NO, KC_NO),     ENCODER_CCW_CW(KC_NO, KC_NO),     ENCODER_CCW_CW(KC_NO, KC_NO),
                 ENCODER_CCW_CW(KC_NO, KC_NO),     ENCODER_CCW_CW(KC_NO, KC_NO),     ENCODER_CCW_CW(KC_NO, KC_NO)  },

    [_LY2] =   { ENCODER_CCW_CW(KC_NO, KC_NO),     ENCODER_CCW_CW(KC_NO, KC_NO),     ENCODER_CCW_CW(KC_NO, KC_NO),
                 ENCODER_CCW_CW(KC_NO, KC_NO),     ENCODER_CCW_CW(KC_NO, KC_NO),     ENCODER_CCW_CW(KC_NO, KC_NO)  },

    [_LY3] =   { ENCODER_CCW_CW(KC_NO, KC_NO),     ENCODER_CCW_CW(KC_NO, KC_NO),     ENCODER_CCW_CW(KC_NO, KC_NO),
                 ENCODER_CCW_CW(KC_NO, KC_NO),     ENCODER_CCW_CW(KC_NO, KC_NO),     ENCODER_CCW_CW(KC_NO, KC_NO)  },

    [_LY4] =   { ENCODER_CCW_CW(KC_NO, KC_NO),     ENCODER_CCW_CW(KC_NO, KC_NO),     ENCODER_CCW_CW(KC_NO, KC_NO),
                 ENCODER_CCW_CW(KC_NO, KC_NO),     ENCODER_CCW_CW(KC_NO, KC_NO),     ENCODER_CCW_CW(KC_NO, KC_NO)  },

    [_LY5] =   { ENCODER_CCW_CW(KC_NO, KC_NO),     ENCODER_CCW_CW(KC_NO, KC_NO),     ENCODER_CCW_CW(KC_NO, KC_NO),
                 ENCODER_CCW_CW(KC_NO, KC_NO),     ENCODER_CCW_CW(KC_NO, KC_NO),     ENCODER_CCW_CW(KC_NO, KC_NO)  },

    [_LY6] =   { ENCODER_CCW_CW(KC_NO, KC_NO),     ENCODER_CCW_CW(KC_NO, KC_NO),     ENCODER_CCW_CW(KC_NO, KC_NO),
                 ENCODER_CCW_CW(KC_NO, KC_NO),     ENCODER_CCW_CW(KC_NO, KC_NO),     ENCODER_CCW_CW(KC_NO, KC_NO)  },

    [_LY7] =   { ENCODER_CCW_CW(KC_NO, KC_NO),     ENCODER_CCW_CW(KC_NO, KC_NO),     ENCODER_CCW_CW(KC_NO, KC_NO),
                 ENCODER_CCW_CW(KC_NO, KC_NO),     ENCODER_CCW_CW(KC_NO, KC_NO),     ENCODER_CCW_CW(KC_NO, KC_NO)  },

    [_LY8] =   { ENCODER_CCW_CW(KC_NO, KC_NO),     ENCODER_CCW_CW(KC_NO, KC_NO),     ENCODER_CCW_CW(KC_NO, KC_NO),
                 ENCODER_CCW_CW(KC_NO, KC_NO),     ENCODER_CCW_CW(KC_NO, KC_NO),     ENCODER_CCW_CW(KC_NO, KC_NO)  },

    [_RGB] =   { ENCODER_CCW_CW(RGB_HUD, RGB_HUI), ENCODER_CCW_CW(RGB_SAD, RGB_SAI), ENCODER_CCW_CW(RGB_VAD, RGB_VAI),
                 ENCODER_CCW_CW(KC_NO,   KC_NO),   ENCODER_CCW_CW(KC_NO,   KC_NO),   ENCODER_CCW_CW(RGB_RMOD, RGB_MOD)  },
};
#endif

void keyboard_post_init_user(void) {
    // Customise these values to desired behaviour
    debug_enable=true;
    debug_matrix=true;
    debug_keyboard=true;
    debug_mouse=true;

    //初始化lvgl
    gpio_set_pin_output(LCD_BL_PIN);
    gpio_write_pin_high(LCD_BL_PIN);
    lcd = qp_st7789_make_spi_device(240, 320, LCD_CS_PIN, LCD_DC_PIN, LCD_RST_PIN, 4, 0);
    qp_init(lcd, QP_ROTATION_270);
    qp_rect(lcd, 0, 0, 239, 319, 0, 0, 255, true);
    qp_flush(lcd);
}

void matrix_init_user(void) {
    type_count = 0;
}

void matrix_scan_user(void) {

}

//
bool encoder_update_user(uint8_t index, bool clockwise) {
    return false;
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    return true;
}

// 层切换
layer_state_t layer_state_set_user(layer_state_t state) {
    return state;
}

// 编码器操作后
void post_encoder_update_user(uint8_t index, bool clockwise) {

}

// 键值操作后
void post_process_record_user(uint16_t keycode, keyrecord_t *record) {

}

bool wpm_keycode_user(uint16_t keycode) {
	return true;
}

// 休眠
void suspend_power_down_user(void) {
    #ifdef RGB_MATRIX_ENABLE
        rgb_matrix_set_suspend_state(true);
    #endif  // RGB_MATRIX_ENABLE
}

// 唤醒
void suspend_wakeup_init_user(void) {
    #ifdef RGB_MATRIX_ENABLE
        rgb_matrix_set_suspend_state(false);
    #endif  // RGB_MATRIX_ENABLE
}
