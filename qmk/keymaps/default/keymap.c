// Copyright 2022 Cole Smith <cole@boadsource.xyz>
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H
#include "split_util.h" 

enum layers {
    _QWERTY,
    _LOWER,
    _RAISE,
    _ADJUST,
    _NUM,
    _LED,
    _L_SLSH, 
    _L_MINS, 
    _L_BSPC,
    _L_QUOT
};


/*
///////////////////////////
// LED Layer definitions //
///////////////////////////

// Light LEDs 6 to 9 and 12 to 15 red when caps lock is active. Hard to ignore!
const PROGMEM my_capslock_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {6, 4, HSV_RED},       // Light 4 LEDs, starting with LED 6
    {12, 4, HSV_RED}       // Light 4 LEDs, starting with LED 12
);
// Light LEDs 9 & 10 in cyan when keyboard layer 1 is active
const rgblight_segment_t PROGMEM my_layer1_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {0, 70, HSV_CYAN}
);
// Light LEDs 11 & 12 in purple when keyboard layer 2 is active
const rgblight_segment_t PROGMEM my_layer2_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {0, 6, HSV_PURPLE}
);
// Light LEDs 13 & 14 in green when keyboard layer 3 is active
const rgblight_segment_t PROGMEM my_layer3_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {35, 6, HSV_GREEN}
);
// Light LEDs 11 & 12 in purple when keyboard layer 2 is active
const rgblight_segment_t PROGMEM my_layer4_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {0, 6, HSV_BLUE},
    {35, 6, HSV_BLUE}
);
// Light LEDs 13 & 14 in green when keyboard layer 3 is active
const rgblight_segment_t PROGMEM my_layer5_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {35, 3, HSV_RED},
    {44, 3, HSV_RED},
    {47, 3, HSV_RED},
    {56, 3, HSV_RED},
    {63, 3, HSV_RED}
);

// Now define the array of layers. Later layers take precedence
const rgblight_segment_t* const PROGMEM my_rgb_layers[] = RGBLIGHT_LAYERS_LIST(
    my_capslock_layer,
    my_layer1_layer,    // Overrides caps lock layer
    my_layer2_layer,    // Overrides other layers
    my_layer3_layer,     // Overrides other layers
    my_layer4_layer,     // Overrides other layers
    my_layer5_layer     // Overrides other layers
);

void keyboard_post_init_user(void) {
    // Enable the LED layers
    rgblight_layers = my_rgb_layers;
}

bool led_update_user(led_t led_state) {
    rgblight_set_layer_state(0, led_state.caps_lock);
    return true;
}


layer_state_t default_layer_state_set_user(layer_state_t state) {
    rgblight_set_layer_state(1, layer_state_cmp(state, _QWERTY));
    return state;
}

layer_state_t layer_state_set_user(layer_state_t state) {
    state = update_tri_layer_state(state, _LOWER, _RAISE, _ADJUST);
    rgblight_set_layer_state(2, layer_state_cmp(state, _LOWER));
    rgblight_set_layer_state(3, layer_state_cmp(state, _RAISE));
    rgblight_set_layer_state(4, layer_state_cmp(state, _ADJUST));
    rgblight_set_layer_state(5, layer_state_cmp(state, _NUM));
    rgblight_set_layer_state(6, layer_state_cmp(state, _LED));
    rgblight_set_layer_state(7, layer_state_cmp(state, _L_SLSH));
    rgblight_set_layer_state(8, layer_state_cmp(state, _L_MINS));
    rgblight_set_layer_state(8, layer_state_cmp(state, _L_BSPC));
    rgblight_set_layer_state(8, layer_state_cmp(state, _L_QUOT));
    return state;
}


//*/

// tapdance keycodes
enum td_keycodes {
  Esc_Reset, // Our example key: `LALT` when held, `(` when tapped. Add additional keycodes for each tapdance.
  TD_Lower,
  TD_Raise
};

// define a type containing as many tapdance states as you need
typedef enum {
  SINGLE_TAP,
  SINGLE_HOLD,
  DOUBLE_SINGLE_TAP
} td_state_t;

typedef struct {
    bool is_press_action;
    td_state_t state;
} td_tap_t;


// create a global instance of the tapdance state type
static td_state_t td_state;

// function to determine the current tapdance state
int cur_dance (tap_dance_state_t *state);

// `finished` and `reset` functions for each tapdance keycode
void esc_reset_finished (tap_dance_state_t *state, void *user_data);
void esc_reset_reset (tap_dance_state_t *state, void *user_data);
void layer_raise_finished (tap_dance_state_t *state, void *user_data);
void layer_raise_reset (tap_dance_state_t *state, void *user_data);
void layer_lower_finished (tap_dance_state_t *state, void *user_data);
void layer_lower_reset (tap_dance_state_t *state, void *user_data);


#define RAISE MO(_RAISE)
#define LOWER MO(_LOWER)

#ifdef OLED_ENABLE
static void render_logo(void) {
    static const char PROGMEM qmk_logo[] = {
        0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F, 0x90, 0x91, 0x92, 0x93, 0x94,
        0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8, 0xA9, 0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF, 0xB0, 0xB1, 0xB2, 0xB3, 0xB4,
        0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7, 0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD, 0xCE, 0xCF, 0xD0, 0xD1, 0xD2, 0xD3, 0xD4, 0x00
    };

    oled_write_P(qmk_logo, false);
}
static void render_status(void) {
// Host Keyboard Layer Status
  oled_write_P(PSTR("Layer: "), false);
  switch (get_highest_layer(layer_state)) {
    case _QWERTY:
      oled_write_P(PSTR("Default\n"), false);
      break;
    case _RAISE:
      oled_write_P(PSTR("Raise\n"), false);
      break;
    case _LOWER:
      oled_write_P(PSTR("Lower\n"), false);
      break;
    case _ADJUST:
      oled_write_P(PSTR("Adjust\n"), false);
      break;
    case _NUM:
      oled_write_P(PSTR("Numpad\n"), false);
      break;
    case _LED:
      oled_write_P(PSTR("LED\n"), false);
      break;
    case _L_SLSH:
      oled_write_P(PSTR("Slash\n"), false);
      break;
    case _L_MINS:
      oled_write_P(PSTR("Minus\n"), false);
      break;
    case _L_BSPC:
      oled_write_P(PSTR("Backspace\n"), false);
      break;
    case _L_QUOT:
      oled_write_P(PSTR("Quote\n"), false);
      break;
    default:
      // Or use the write_ln shortcut over adding '\n' to the end of your string
      oled_write_ln_P(PSTR("Undefined"), false);
  }

  // Host Keyboard LED Status
  led_t led_state = host_keyboard_led_state();
  oled_write_P(led_state.num_lock ? PSTR("NUM ") : PSTR("    "), false);
  oled_write_P(led_state.caps_lock ? PSTR("CAP ") : PSTR("    "), false);
  oled_write_P(led_state.scroll_lock ? PSTR("SCR ") : PSTR("    "), false);
}

bool oled_task_user(void) {
    if (is_keyboard_master()) {
        render_status();  // Renders the current keyboard state (layer, lock, caps, scroll, etc)
    } else {
        render_logo();  // Renders a static logo
        oled_scroll_left();  // Turns on scrolling
    }
    return false;
}

#endif


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
 
/* QWERTY
              ╭───────────┬───────────┬───────────┬───────────┬───────────╮     ╭───────────┬───────────┬───────────┬───────────┬───────────╮
╭─────────────┤     Q     │     W     │    E      │     R     │     T     │     │     Y     │     U     │     I     │     O     │     P     ├───────────╮
│    Esc*     ├───────────┼───────────┼───────────┼───────────┼───────────┤     ├───────────┼───────────┼───────────┼───────────┼───────────┤  Bspace (l)   
├─────────────┤     A     │     S     │    D      │     F     │     G     │     │     H     │     J     │     K     │     L     │     - (l) ├───────────┤
│ LShift/Tab  ├───────────┼───────────┼───────────┼───────────┼───────────┤     ├───────────┼───────────┼───────────┼───────────┼───────────┤    '  (l) │
╰─────────────┤     Z     │     X     │    C      │     V     │     B     │     │     N     │     M     │    , <    │    . >    │   / ? (l) ├───────────╯
              ╰───────────┴───────────┴───────────┴───────────┴───────────╯     ╰───────────┴───────────┴───────────┴───────────┴───────────╯
                          ╭───────────┬───────────┬───────────┬───────────╮     ╭───────────┬───────────┬───────────┬───────────╮
                          │           │           │           │           │     │           │           │           │                                      
                          ╰───────────┴───────────┤           ├───────────╯     ╰───────────┤           ├───────────┴───────────╯
                                  						    ╰───────────╯                             ╰───────────╯         
 */

 [_QWERTY] = LAYOUT(
        TD(Esc_Reset),   KC_Q,   KC_W,    KC_E,    KC_R,    KC_T,        KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    LT(_L_BSPC, KC_BSPC),
  MT(MOD_LSFT, KC_TAB),  KC_A,   KC_S,    KC_D,    KC_F,    KC_G,        KC_H,    KC_J,    KC_K,    KC_L,    LT(_L_MINS, KC_MINS), LT(_L_QUOT, KC_QUOT),
                         KC_Z,   KC_X,    KC_C,    KC_V,    KC_B,        KC_N,    KC_M,    KC_COMM, KC_DOT,  LT(_L_SLSH, KC_SLSH),  
                                TG(_NUM),   LOWER,   KC_ENT,  KC_LCTL,     KC_RALT, KC_SPACE, RAISE, MO(_LED)
),

/* LOWER
              ╭───────────┬───────────┬───────────┬───────────┬───────────╮     ╭───────────┬───────────┬───────────┬───────────┬───────────╮
╭─────────────┤     \     │     -     │     7     │     8     │     9     │     │           │     _     │     |     │      %    │    +      ├───────────╮
│ Esc + LayEsc├───────────┼───────────┼───────────┼───────────┼───────────┤     ├───────────┼───────────┼───────────┼───────────┼───────────┤     
├─────────────┤     *     │     +     │     4     │     5     │     6     │     │     #     │           │     (     │      )    │           ├───────────┤
│ LShift+Tab  ├───────────┼───────────┼───────────┼───────────┼───────────┤     ├───────────┼───────────┼───────────┼───────────┼───────────┤           │
╰─────────────┤     =     │     0     │     1     │     2     │     3     │     │     &     │     -     │     [{    │     ]}    │           ├───────────╯
              ╰───────────┴───────────┴───────────┴───────────┴───────────╯     ╰───────────┴───────────┴───────────┴───────────┴───────────╯
                          ╭───────────┬───────────┬───────────┬───────────╮     ╭───────────┬───────────┬───────────┬───────────╮
                          │           │           │           │           │     │           │           │           │                                      
                          ╰───────────┴───────────┤           ├───────────╯     ╰───────────┤           ├───────────┴───────────╯
                                  						    ╰───────────╯                             ╰───────────╯
 */
[_LOWER] = LAYOUT(
       TD(Esc_Reset), KC_BSLS, KC_MINS, KC_7, KC_8, KC_9,                   _______, KC_UNDS, KC_PIPE, KC_PERC, KC_PLUS, _______,
MT(MOD_LSFT, KC_TAB), KC_PAST, KC_PLUS, KC_4, KC_5, KC_6,                   KC_HASH, _______, KC_LPRN, KC_RPRN, _______, _______,
                      KC_EQL,  KC_0,    KC_1, KC_2, KC_3,                   KC_AMPR, KC_MINS, KC_LBRC, KC_RBRC, _______,
                      _______, _______, _______, _______, _______,  _______, _______, _______
),
/* RAISE
              ╭───────────┬───────────┬───────────┬───────────┬───────────╮     ╭───────────┬───────────┬───────────┬───────────┬───────────╮
╭─────────────┤     ?     │           │    =      │           │     ~     │     │  PrintSc  │ ScrollLock│Pause/Break│  CapsLock │           ├───────────╮
│             ├───────────┼───────────┼───────────┼───────────┼───────────┤     ├───────────┼───────────┼───────────┼───────────┼───────────┤  Delete   │ 
├─────────────┤     @     │   * (star)│    $      │           │   ` ~     │     │  Insert   │   Home    │ Page Up   │           │           ├───────────┤
│   LSHIFT    ├───────────┼───────────┼───────────┼───────────┼───────────┤     ├───────────┼───────────┼───────────┼───────────┼───────────┤           │
╰─────────────┤           │    !      │    ^      │    {      │     }     │     │  Delete   │   End     │ Page Down │ContextMenu│           ├───────────╯
              ╰───────────┴───────────┴───────────┴───────────┴───────────╯     ╰───────────┴───────────┴───────────┴───────────┴───────────╯
                          ╭───────────┬───────────┬───────────┬───────────╮     ╭───────────┬───────────┬───────────┬───────────╮
                          │           │           │           │   Mods    │     │   Mods    │           │           │                                      
                          ╰───────────┴───────────┤           ├───────────╯     ╰───────────┤           ├───────────┴───────────╯
 						                                      ╰───────────╯                             ╰───────────╯
 */

[_RAISE] = LAYOUT(
       TD(Esc_Reset), KC_QUES, _______, KC_EQL, _______, KC_TILD,            KC_PSCR, KC_SCRL, KC_PAUS, KC_CAPS, _______, KC_DEL,
MT(MOD_LSFT, KC_TAB), KC_AT,   KC_ASTR, KC_DLR, _______, KC_GRV,             KC_INS,  KC_HOME, KC_PGUP, _______, _______, _______,
                      _______, KC_EXLM, KC_CIRC, KC_LCBR, KC_RCBR,                KC_DEL,  KC_END, KC_PGDN, KC_APP,   _______, 
                              _______,  _______, _______,  _______,          KC_RCTL,  _______, _______, _______
),

/* ADJUST
              ╭───────────┬───────────┬───────────┬───────────┬───────────╮     ╭───────────┬───────────┬───────────┬───────────┬───────────╮
╭─────────────┤    F1     │    F2     │    F3     │    F4     │           │     │ PROFILE 0 │ PROFILE 2 │ PROFILE 4 │INC BRIGHT │ VOL UP    ├───────────╮
│             ├───────────┼───────────┼───────────┼───────────┼───────────┤     ├───────────┼───────────┼───────────┼───────────┼───────────┤ NEXTSONG     
├─────────────┤    F5     │    F6     │    F7     │    F8     │           │     │ PROFILE 1 │ PROFILE 3 │ TOGGLEOUT │ DEC BRIGHT │ VOL DOWN ├───────────┤
│   LSHIFT    ├───────────┼───────────┼───────────┼───────────┼───────────┤     ├───────────┼───────────┼───────────┼───────────┼───────────┤ PAUSEMUSIC│
╰─────────────┤    F9     │    F10    │    F11    │    F12    │           │     │ CLEAR BT  │           │           │  VOL MUTE │  PREVSONG ├───────────╯
              ╰───────────┴───────────┴───────────┴───────────┴───────────╯     ╰───────────┴───────────┴───────────┴───────────┴───────────╯
                          ╭───────────┬───────────┬───────────┬───────────╮     ╭───────────┬───────────┬───────────┬───────────╮
                          │           │           │           │           │     │           │           │           │                                      
                          ╰───────────┴───────────┤           ├───────────╯     ╰───────────┤           ├───────────┴───────────╯
 						                                      ╰───────────╯                             ╰───────────╯
 */
  [_ADJUST] = LAYOUT(
       TD(Esc_Reset), XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
MT(MOD_LSFT, KC_TAB), XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, 
                              _______, _______, _______, _______,                    _______,  _______, _______, _______
  ),

  /* Numpad
              ╭───────────┬───────────┬───────────┬───────────┬───────────╮     ╭───────────┬───────────┬───────────┬───────────┬───────────╮
╭─────────────┤           │   Edit    │           │ContextMenu│           │     │  7 (Home) │     8     │  9 (PgUp) │ De-colour │   Space   ├───────────╮
│             ├───────────┼───────────┼───────────┼───────────┼───────────┤     ├───────────┼───────────┼───────────┼───────────┼───────────┤ Backspace   
├─────────────┤           │           │           │           │  General  │     │     4     │     5     │     6     │   Time    │   Date    ├───────────┤
│   LSHIFT    ├───────────┼───────────┼───────────┼───────────┼───────────┤     ├───────────┼───────────┼───────────┼───────────┼───────────┤ Numlk     │
╰─────────────┤     Z     │     X     │     C     │     V     │           │     │  1 (End)  │     2     │  3 (PgDn) │  :  Ctrl  │  Shift    ├───────────╯
              ╰───────────┴───────────┴───────────┴───────────┴───────────╯     ╰───────────┴───────────┴───────────┴───────────┴───────────╯
                          ╭───────────┬───────────┬───────────┬───────────╮     ╭───────────┬───────────┬───────────┬───────────╮
                          │           │           │           │           │     │  Enter    │   Tab     │  0 (Ins)  │   . (Del) │
                          ╰───────────┴───────────┤           ├───────────╯     ╰───────────┤           ├───────────┴───────────╯
                                  						    ╰───────────╯                             ╰───────────╯
*/
  [_NUM] = LAYOUT(
       TD(Esc_Reset), XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                   KC_P7, KC_P8, KC_P9, XXXXXXX, XXXXXXX, XXXXXXX,
MT(MOD_LSFT, KC_TAB), XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                   KC_P4, KC_P5, KC_P6, RCS(KC_SCLN), C(KC_SCLN), XXXXXXX,
             XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                   KC_P1, KC_P2, KC_P3, KC_COLN, XXXXXXX, 
 // XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
                      TG(_NUM),  _______, _______, _______,                  _______, _______, _______, _______
  ),

/* LED
              ╭───────────┬───────────┬───────────┬───────────┬───────────╮     ╭───────────┬───────────┬───────────┬───────────┬───────────╮
╭─────────────┤           │           │           │           │           │     │           │           │           │           │           ├───────────╮
│             ├───────────┼───────────┼───────────┼───────────┼───────────┤     ├───────────┼───────────┼───────────┼───────────┼───────────┤     
├─────────────┤           │           │           │           │           │     │           │           │           │           │           ├───────────┤
│             ├───────────┼───────────┼───────────┼───────────┼───────────┤     ├───────────┼───────────┼───────────┼───────────┼───────────┤           │
╰─────────────┤           │           │           │           │           │     │           │           │           │           │           ├───────────╯
              ╰───────────┴───────────┴───────────┴───────────┴───────────╯     ╰───────────┴───────────┴───────────┴───────────┴───────────╯
                          ╭───────────┬───────────┬───────────┬───────────╮     ╭───────────┬───────────┬───────────┬───────────╮
                          │           │           │           │           │     │           │           │           │                                      
                          ╰───────────┴───────────┤           ├───────────╯     ╰───────────┤           ├───────────┴───────────╯
                                  						    ╰───────────╯                             ╰───────────╯
*/
  [_LED] = LAYOUT(
       TD(Esc_Reset), RGB_M_P, RGB_M_B, RGB_M_R, RGB_M_SW, RGB_M_SN,                   RGB_TOG, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
MT(MOD_LSFT, KC_TAB), RGB_M_K, RGB_M_X, RGB_M_G, RGB_M_T, RGB_M_TW,                   RGB_MOD, RGB_HUI, RGB_SAI, RGB_VAI, XXXXXXX, XXXXXXX,
             XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                   RGB_RMOD, RGB_HUD, RGB_SAD, RGB_VAD, XXXXXXX,  
 // XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
                      _______, _______, _______, _______,                    _______,  _______, _______, _______
  ),

/* Backspace Layer
              ╭───────────┬───────────┬───────────┬───────────┬───────────╮     ╭───────────┬───────────┬───────────┬───────────┬───────────╮
╭─────────────┤           │           │           │           │           │     │   Redo    │   Undo    │           │           │           ├───────────╮
│             ├───────────┼───────────┼───────────┼───────────┼───────────┤     ├───────────┼───────────┼───────────┼───────────┼───────────┤     
├─────────────┤           │           │           │           │           │     │           │ Del ←Word │    Del    │ Del →Word │           ├───────────┤
│             ├───────────┼───────────┼───────────┼───────────┼───────────┤     ├───────────┼───────────┼───────────┼───────────┼───────────┤           │
╰─────────────┤           │           │           │           │           │     │           │           │           │           │           ├───────────╯
              ╰───────────┴───────────┴───────────┴───────────┴───────────╯     ╰───────────┴───────────┴───────────┴───────────┴───────────╯
                          ╭───────────┬───────────┬───────────┬───────────╮     ╭───────────┬───────────┬───────────┬───────────╮
                          │           │           │           │           │     │           │           │           │                                      
                          ╰───────────┴───────────┤           ├───────────╯     ╰───────────┤           ├───────────┴───────────╯
                                  						    ╰───────────╯                             ╰───────────╯
*/
  [_L_BSPC] = LAYOUT(
       TD(Esc_Reset), XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                   C(KC_Y), C(KC_Z), XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
MT(MOD_LSFT, KC_TAB), XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                   XXXXXXX, C(KC_BSPC),  KC_DEL, C(KC_DEL), XXXXXXX, XXXXXXX,
             XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,  
                      _______, _______, _______, _______, _______,  _______, _______, _______
  ),

/* Minus Layer
              ╭───────────┬───────────┬───────────┬───────────┬───────────╮     ╭───────────┬───────────┬───────────┬───────────┬───────────╮
╭─────────────┤           │           │           │           │           │     │           │     _     │           │           │           ├───────────╮
│             ├───────────┼───────────┼───────────┼───────────┼───────────┤     ├───────────┼───────────┼───────────┼───────────┼───────────┤     
├─────────────┤           │           │           │           │           │     │           │           │     =     │     +     │           ├───────────┤
│             ├───────────┼───────────┼───────────┼───────────┼───────────┤     ├───────────┼───────────┼───────────┼───────────┼───────────┤           │
╰─────────────┤           │           │           │           │           │     │           │           │           │           │           ├───────────╯
              ╰───────────┴───────────┴───────────┴───────────┴───────────╯     ╰───────────┴───────────┴───────────┴───────────┴───────────╯
                          ╭───────────┬───────────┬───────────┬───────────╮     ╭───────────┬───────────┬───────────┬───────────╮
                          │           │           │           │           │     │           │           │           │                                      
                          ╰───────────┴───────────┤           ├───────────╯     ╰───────────┤           ├───────────┴───────────╯
                                  						    ╰───────────╯                             ╰───────────╯
*/
  [_L_MINS] = LAYOUT(
       TD(Esc_Reset), XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                   XXXXXXX, KC_UNDS, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
MT(MOD_LSFT, KC_TAB), XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                   XXXXXXX, XXXXXXX, KC_EQL,  KC_PLUS, XXXXXXX, XXXXXXX,
             XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, 
                      _______, _______,  _______, _______, _______,  _______, _______, _______
  ),


/* Slash Layer
              ╭───────────┬───────────┬───────────┬───────────┬───────────╮     ╭───────────┬───────────┬───────────┬───────────┬───────────╮
╭─────────────┤           │           │           │           │           │     │           │           │           │           │           ├───────────╮
│             ├───────────┼───────────┼───────────┼───────────┼───────────┤     ├───────────┼───────────┼───────────┼───────────┼───────────┤     
├─────────────┤           │           │           │           │           │     │     ]     │     (     │     $     │     )     │           ├───────────┤
│             ├───────────┼───────────┼───────────┼───────────┼───────────┤     ├───────────┼───────────┼───────────┼───────────┼───────────┤           │
╰─────────────┤           │           │           │           │           │     │     [     │     {     │     \     │     }     │           ├───────────╯
              ╰───────────┴───────────┴───────────┴───────────┴───────────╯     ╰───────────┴───────────┴───────────┴───────────┴───────────╯
                          ╭───────────┬───────────┬───────────┬───────────╮     ╭───────────┬───────────┬───────────┬───────────╮
                          │           │           │           │           │     │           │           │           │                                      
                          ╰───────────┴───────────┤           ├───────────╯     ╰───────────┤           ├───────────┴───────────╯
                                  						    ╰───────────╯                             ╰───────────╯
*/
  [_L_SLSH] = LAYOUT(
       TD(Esc_Reset), XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
MT(MOD_LSFT, KC_TAB), XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                   KC_LBRC, KC_LPRN, KC_DLR, KC_RPRN, XXXXXXX, XXXXXXX,
             XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                   KC_RBRC, KC_LCBR, KC_BSLS, KC_RCBR, XXXXXXX, 
                      _______, _______,   _______, _______, _______,  _______, _______, _______
  ),

/* Single Quote Layer
              ╭───────────┬───────────┬───────────┬───────────┬───────────╮     ╭───────────┬───────────┬───────────┬───────────┬───────────╮
╭─────────────┤           │           │           │           │           │     │  Ctrl(←)  │  Ctrl(↓)  │  Ctrl(↑)  │  Ctrl(→)  │           ├───────────╮
│             ├───────────┼───────────┼───────────┼───────────┼───────────┤     ├───────────┼───────────┼───────────┼───────────┼───────────┤     
├─────────────┤           │           │           │           │           │     │     ←     │     ↓     │     ↑     │    →      │           ├───────────┤
│             ├───────────┼───────────┼───────────┼───────────┼───────────┤     ├───────────┼───────────┼───────────┼───────────┼───────────┤           │
╰─────────────┤           │           │           │           │           │     │   HOME    │  PAGE DN  │  PAGE UP  │    END    │           ├───────────╯
              ╰───────────┴───────────┴───────────┴───────────┴───────────╯     ╰───────────┴───────────┴───────────┴───────────┴───────────╯
                          ╭───────────┬───────────┬───────────┬───────────╮     ╭───────────┬───────────┬───────────┬───────────╮
                          │           │           │           │           │     │           │           │           │                                      
                          ╰───────────┴───────────┤           ├───────────╯     ╰───────────┤           ├───────────┴───────────╯
                                  						    ╰───────────╯                             ╰───────────╯
*/
  [_L_QUOT] = LAYOUT(
       TD(Esc_Reset), XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                   C(KC_LEFT), C(KC_DOWN), C(KC_UP), C(KC_RGHT), XXXXXXX, XXXXXXX,
MT(MOD_LSFT, KC_TAB), XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                   KC_LEFT,    KC_DOWN,    KC_UP,    KC_RGHT,    XXXXXXX, XXXXXXX,
             XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                   KC_HOME,    KC_PGDN,    KC_PGUP,  KC_END,     XXXXXXX, 
                      _______,  _______, _______, _______,                  _______,    KC_RSFT,    _______,  _______
  )
};




layer_state_t layer_state_set_user(layer_state_t state) {
   return update_tri_layer_state(state, _LOWER, _RAISE, _ADJUST);
}



// determine the tapdance state to return
int cur_dance (tap_dance_state_t *state) {
  if (state->count == 1) {
    if (state->interrupted || !state->pressed) { return SINGLE_TAP; }
    else { return SINGLE_HOLD; }
  }
  if (state->count == 2) { return DOUBLE_SINGLE_TAP; }
  else { return 3; } // any number higher than the maximum state value you return above
}
 
// handle the possible states for each tapdance keycode you define:

void esc_reset_finished (tap_dance_state_t *state, void *user_data) {
  td_state = cur_dance(state);
  switch (td_state) {
    case SINGLE_TAP:
      register_code16(KC_ESC);
      break;
    case SINGLE_HOLD:
      register_code16(KC_ESC); // for a layer-tap key, use `layer_on(_MY_LAYER)` here
      break;
    case DOUBLE_SINGLE_TAP: 
      layer_move(_QWERTY);
  }
}

void esc_reset_reset (tap_dance_state_t *state, void *user_data) {
  switch (td_state) {
    case SINGLE_TAP:
      unregister_code16(KC_ESC);
      break;
    case SINGLE_HOLD:
      unregister_code16(KC_ESC); // for a layer-tap key, use `layer_off(_MY_LAYER)` here
      break;
    case DOUBLE_SINGLE_TAP:
      break;
  }
}

void layer_raise_finished (tap_dance_state_t *state, void *user_data) {
  td_state = cur_dance(state);
  switch (td_state) {
    case SINGLE_TAP:
      layer_on(_LED);
      break;
    case SINGLE_HOLD:
      layer_on(_RAISE);  // for a layer-tap key, use `layer_on(_MY_LAYER)` here
      break;
    default:
      break;
  }
}

void layer_raise_reset (tap_dance_state_t *state, void *user_data) {
  switch (td_state) {
    case SINGLE_TAP:
      break;
    case SINGLE_HOLD:
      layer_off(_RAISE); // for a layer-tap key, use `layer_off(_MY_LAYER)` here
      break;
    default:
      break;
  }
}

void layer_lower_finished (tap_dance_state_t *state, void *user_data) {
  td_state = cur_dance(state);
  switch (td_state) {
    case SINGLE_TAP:
      layer_on(_NUM);
      break;
    case SINGLE_HOLD:
      layer_on(_LOWER);  // for a layer-tap key, use `layer_on(_MY_LAYER)` here
      break;
    default:
      break;
  }
}

void layer_lower_reset (tap_dance_state_t *state, void *user_data) {
  switch (td_state) {
    case SINGLE_TAP:
      break;
    case SINGLE_HOLD:
      layer_off(_LOWER); // for a layer-tap key, use `layer_off(_MY_LAYER)` here
      break;
    default:
      break;
  }
}



// define `ACTION_TAP_DANCE_FN_ADVANCED()` for each tapdance keycode, passing in `finished` and `reset` functions
tap_dance_action_t tap_dance_actions[] = {
  [Esc_Reset] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, esc_reset_finished, esc_reset_reset)
};