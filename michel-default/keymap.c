// Copyright 2023 QMK
// SPDX-License-Identifier: GPL-2.0-or-later
#include QMK_KEYBOARD_H

#define SPC_NAV  LT(_NAV, KC_SPC)
#define ENT_NUM  LT(_NUM, KC_ENT)
#define BCSP_SYM  LT(_SYM, KC_BSPC)

#define ESC_GUI  MT(MOD_LGUI, KC_ESC)
#define SCLN_GUI MT(MOD_RGUI, KC_SCLN)
#define EQUAL_SHFT  MT(MOD_LSFT, KC_EQUAL)

enum sofle_layers {
    /* _M_XYZ = Mac Os, _W_XYZ = Win/Linux */
    _BASE,
    _NUM,
    _SYM,
    _FUN,
    _NAV,
    _MOU,
};

enum custom_keycodes {
    TMUX_SHFT = SAFE_RANGE,
    // QUOTE_GRV,
    MOU_BSPC,
};

typedef struct {
    bool     down;
    bool     hold;
    uint16_t timer;
} dual_state_t;

// typedef struct {
//     bool     down;
//     uint16_t timer;
// } single_state_t;

static dual_state_t tmux = {0};
static dual_state_t mb = {0};
// static single_state_t qg = {0};

const uint16_t PROGMEM left_ctrl[] = {KC_S, KC_T, COMBO_END};
const uint16_t PROGMEM left_alt[] = {KC_T, KC_D, COMBO_END};
const uint16_t PROGMEM right_ctrl[] = {KC_N, KC_E, COMBO_END};
const uint16_t PROGMEM right_alt[] = {KC_N, KC_H, COMBO_END};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_BASE] = LAYOUT(
      KC_CAPS,   KC_1,   KC_2,    KC_3,    KC_4,    KC_5,                      KC_6,    KC_7,    KC_8,    KC_9,    KC_0,  KC_DEL,
      KC_TAB,   KC_Q,   KC_W,    KC_F,    KC_P,    KC_G,                      KC_J,    KC_L,    KC_U,    KC_Y, KC_QUOT,  KC_BSLS,
      ESC_GUI,   KC_A,   KC_R,    KC_S,    KC_T,    KC_D,                      KC_H,    KC_N,    KC_E,    KC_I,    KC_O,  SCLN_GUI,
      TMUX_SHFT,  KC_Z,   KC_X,    KC_C,    KC_V,    KC_B, KC_MUTE,      XXXXXXX,KC_K,    KC_M, KC_COMM,  KC_DOT, KC_SLSH,  EQUAL_SHFT,
                     KC_LCTL, KC_LALT, _______, MOU_BSPC, SPC_NAV,        ENT_NUM,  BCSP_SYM, _______, KC_RALT, KC_RCTL
    ),
    [_NAV] = LAYOUT(
      _______, _______, _______, _______, _______, _______,                   KC_F7, KC_F8, KC_F9, KC_F10, KC_F11, KC_F12,
      _______, _______, _______, _______, _______, _______,                   KC_PSCR, _______, _______, _______, _______, _______,
      _______, KC_LGUI, KC_LALT, KC_LCTL, KC_LSFT, KC_BSPC,                   _______, KC_LEFT, KC_DOWN, KC_UP, KC_RIGHT, _______,
      _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_HOME, KC_PGDN, KC_PGUP, KC_END, _______,
                         _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
    ),
    [_SYM] = LAYOUT(
      _______, _______, _______, _______, _______, _______,                   _______, _______, _______, _______, _______, _______,
      _______, KC_LBRC, KC_AMPR, KC_ASTR, KC_GRV, KC_RBRC,                   _______, _______, _______, _______, _______, _______,
      _______, KC_LCBR, KC_DLR, KC_PERC, KC_CIRC, KC_RCBR,                   KC_BSPC, KC_RSFT, KC_RCTL, KC_RALT, KC_RGUI, _______,
      _______, KC_LPRN,  KC_EXLM, KC_AT, KC_HASH, KC_RPRN, _______, _______, _______, _______, _______, _______, _______,  _______,
                         _______, _______, KC_PLUS, KC_MINS, KC_UNDS, _______, _______, _______, _______, _______
    ),
    [_NUM] = LAYOUT(
      KC_F1, KC_F2, KC_F3, KC_F4,  KC_F5, KC_F6,                   _______, _______, _______, _______, _______, _______,
      _______, KC_SLSH, KC_7, KC_8, KC_9, KC_PLUS,                   _______, _______, _______, _______, _______, _______,
      _______, KC_ASTR, KC_4, KC_5, KC_6, KC_MINS,                   KC_BSPC, KC_RSFT, KC_RCTL, KC_RALT, KC_RGUI, _______,
      _______, KC_DOT,  KC_1, KC_2, KC_3, KC_EQUAL, _______, _______, _______, _______, _______, _______, _______,  _______,
                         _______, _______, _______, KC_0, _______, _______, _______, _______, _______, _______
    ),
    [_MOU] = LAYOUT(
      _______, _______, _______, _______, _______, _______,                   _______, _______, _______, _______, _______, _______,
      _______, _______, _______, _______, _______, _______,                   _______, _______, _______, _______, _______, _______,
      _______, KC_LGUI, KC_LALT, KC_LCTL, KC_LSFT, KC_BSPC,                   _______, MS_LEFT, MS_DOWN, MS_UP, MS_RGHT, _______,
      _______, _______, _______, _______, _______, _______, _______, _______, _______, MS_WHLL, MS_WHLD, MS_WHLU, MS_WHLR, _______,
                         _______, _______, _______, _______, _______, MS_BTN1, MS_BTN2, MS_BTN3, _______, _______
    ),
};

combo_t key_combos[] = {
    COMBO(left_ctrl, KC_LCTL),
    COMBO(left_alt, KC_LALT),
    COMBO(right_ctrl, KC_RCTL),
    COMBO(right_alt, KC_RALT),
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case TMUX_SHFT:
            if (record->event.pressed) {
                tmux.down  = true;
                tmux.hold  = false;
                tmux.timer = timer_read();
            } else {
                if (!tmux.hold && timer_elapsed(tmux.timer) < TAPPING_TERM) {
                    register_mods(MOD_BIT(KC_LCTL));
                    tap_code(KC_B);
                    unregister_mods(MOD_BIT(KC_LCTL));
                } else {
                    if (tmux.hold) {
                        unregister_mods(MOD_BIT(KC_RSFT));
                    }
                }
                tmux.down = false;
            }
            return false; // handled

        // case QUOTE_GRV:
        //     if (record->event.pressed) {
        //         qg.down  = true;
        //         qg.timer = timer_read();
        //     } else {
        //         if (timer_elapsed(qg.timer) < TAPPING_TERM) {
        //             tap_code(KC_QUOT);
        //         } else {
        //             tap_code(KC_GRV);
        //         }
        //         qg.down = false;
        //     }
        //     return false; // handled

        case MOU_BSPC:
            if (record->event.pressed) {
                mb.down = true;
                mb.hold = false;
                mb.timer = timer_read();
            } else {
                if (!mb.hold && timer_elapsed(mb.timer) < TAPPING_TERM) {
                    register_mods(MOD_BIT(KC_LCTL));
                    tap_code(KC_BSPC);
                    unregister_mods(MOD_BIT(KC_LCTL));
                } else {
                    if (mb.hold) {
                        layer_off(_MOU);
                    }
                }
                mb.down = false;
            }
            return false;

        default:
            // Convert to HOLD the moment another key is pressed while the key is down
            if (record->event.pressed) {
                if (tmux.down && !tmux.hold) {
                    register_mods(MOD_BIT(KC_RSFT));
                    tmux.hold = true;
                } else if (mb.down && !mb.hold) {
                    layer_on(_MOU);
                    mb.hold = true;
                }
            }
            return true; // let other keys process normally
    }
}
