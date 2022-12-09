/*
Copyright 2019 @foostan
Copyright 2020 Drashna Jaelre <@drashna>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include QMK_KEYBOARD_H
#include <time.h>

#ifdef OLED_ENABLE
oled_rotation_t oled_init_user(oled_rotation_t rotation) {
  if (!is_keyboard_master()) {
    return OLED_ROTATION_180;  // flips the display 180 degrees if offhand
  }
  return rotation;
}

#define L_BASE 0
#define L_NUMS 2
#define L_SYMBOLS 4
#define L_MEDIA 8
#define L_GAMES 16
#define L_QWERTY 32

void oled_render_layer_state(void) {
    oled_write_P(PSTR("Layer: "), false);
    switch (layer_state) {
        case L_BASE:
            oled_write_P(PSTR("Dvorak\n"), false);
            break;
        case L_NUMS:
            oled_write_P(PSTR("Numbers\n"), false);
            break;
        case L_SYMBOLS:
            oled_write_P(PSTR("Symbols\n"), false);
            break;
        case L_MEDIA:
            oled_write_P(PSTR("Media\n"), false);
            break;
        case L_GAMES:
            oled_write_P(PSTR("Games\n"), false);
            break;
        case L_QWERTY:
            oled_write_P(PSTR("QWERTY\n"), false);
            break;
		default:
            oled_write_ln_P(PSTR("Dvorak"), false);
    }
}

uint16_t keys_pressed = 1;

char keys_pressed_str[6];

void oled_render_keylog(char* keylog) {
    oled_write("KeyCount: ", false);
    oled_write(keylog, false);
}

void oled_render_time(void) {
    time_t now = time(NULL);
    struct tm *tm_struct = localtime(&now);

    int hour = tm_struct->tm_hour;
    int minute = tm_struct->tm_min;

    char hour_str[2];
    char minute_str[2];

    itoa(hour, hour_str, 10);
    itoa(minute, minute_str, 10);

    oled_write(hour_str, false);
    //oled_write_P(":", false);
    //oled_write_P(minute_str, false);
}

void render_bootmagic_status(bool status) {
    /* Show Ctrl-Gui Swap options */
    static const char PROGMEM logo[][2][3] = {
        {{0x97, 0x98, 0}, {0xb7, 0xb8, 0}},
        {{0x95, 0x96, 0}, {0xb5, 0xb6, 0}},
    };
    if (status) {
        oled_write_ln_P(logo[0][0], false);
        oled_write_ln_P(logo[0][1], false);
    } else {
        oled_write_ln_P(logo[1][0], false);
        oled_write_ln_P(logo[1][1], false);
    }
}

void oled_render_logo(void) {
    static const char PROGMEM crkbd_logo[] = {
        0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8a, 0x8b, 0x8c, 0x8d, 0x8e, 0x8f, 0x90, 0x91, 0x92, 0x93, 0x94,
        0xa0, 0xa1, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7, 0xa8, 0xa9, 0xaa, 0xab, 0xac, 0xad, 0xae, 0xaf, 0xb0, 0xb1, 0xb2, 0xb3, 0xb4,
        0xc0, 0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xcb, 0xcc, 0xcd, 0xce, 0xcf, 0xd0, 0xd1, 0xd2, 0xd3, 0xd4,
        0};
    oled_write_P(crkbd_logo, false);
}

bool oled_task_user(void) {
    if (is_keyboard_master()) {
        oled_render_layer_state();
        oled_render_keylog(keys_pressed_str);
    } else {
        oled_render_time();
    }
    return false;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  if (record->event.pressed) {
    itoa(keys_pressed++, keys_pressed_str, 10);
  }
  return true;
}

#endif // OLED_ENABLE
