/**
 * PS/2 keyboard interface control class (Version 0.0.1)
 *
 * Copyright (C) 2010 Shinichiro Nakamura (CuBeatSystems)
 * http://shinta.main.jp/
 */
#include "PS2Keyboard.h"

PS2Keyboard::PS2Keyboard(PinName clk_pin, PinName dat_pin)
        : ps2kb_init(clk_pin, dat_pin), ps2kb(clk_pin, dat_pin) {
}

PS2Keyboard::~PS2Keyboard() {
}

bool PS2Keyboard::processing(keyboard_event_t *p) {
    bool emit = false;
    const int c = ps2kb.getc();
    if (0 <= c) {
        scancode[count++] = c;
        switch (count) {
            case 1:
                if ((scancode[0] != 0xE0)
                        && (scancode[0] != 0xE1)
                        && (scancode[0] != 0xF0)) {
                    p->type = KeyMake;
                    p->length = count;
                    memcpy(p->scancode, scancode, sizeof(p->scancode));
                    emit = true;
                    count = 0;
                }
                break;
            case 2:
                if (scancode[0] == 0xF0) {
                    p->type = KeyBreak;
                    p->length = count;
                    memcpy(p->scancode, scancode, sizeof(p->scancode));
                    emit = true;
                    count = 0;
                }
                if ((scancode[0] == 0xE0)
                        && (scancode[1] != 0xF0)
                        && (scancode[1] != 0x12)) {
                    p->type = KeyMake;
                    p->length = count;
                    memcpy(p->scancode, scancode, sizeof(p->scancode));
                    emit = true;
                    count = 0;
                }
                break;
            case 3:
                if ((scancode[0] == 0xE0)
                        && (scancode[1] == 0xF0)
                        && (scancode[2] != 0x7C)) {
                    p->type = KeyBreak;
                    p->length = count;
                    memcpy(p->scancode, scancode, sizeof(p->scancode));
                    emit = true;
                    count = 0;
                }
                break;
            case 4:
                if ((scancode[0] == 0xE0)
                        && (scancode[1] == 0x12)
                        && (scancode[2] == 0xE0)
                        && (scancode[3] == 0x7C)) {
                    p->type = KeyMake;
                    p->length = count;
                    memcpy(p->scancode, scancode, sizeof(p->scancode));
                    emit = true;
                    count = 0;
                }
                break;
            case 5:
                // Do nothing.
                break;
            case 6:
                if ((scancode[0] == 0xE0)
                        && (scancode[1] == 0xF0)
                        && (scancode[2] == 0x7C)
                        && (scancode[3] == 0xE0)
                        && (scancode[4] == 0xF0)
                        && (scancode[5] == 0x12)) {
                    p->type = KeyBreak;
                    p->length = count;
                    memcpy(p->scancode, scancode, sizeof(p->scancode));
                    emit = true;
                    count = 0;
                }
                break;
            case 7:
                // Do nothing.
                break;
            case 8:
                if ((scancode[0] == 0xE1)
                        && (scancode[1] == 0x14)
                        && (scancode[2] == 0x77)
                        && (scancode[3] == 0xE1)
                        && (scancode[4] == 0xF0)
                        && (scancode[5] == 0x14)
                        && (scancode[6] == 0xF0)
                        && (scancode[7] == 0x77)) {
                    p->type = KeyMake;
                    p->length = count;
                    memcpy(p->scancode, scancode, sizeof(p->scancode));
                    emit = true;
                    count = 0;
                }
                break;
            default:
                count = 0;
                break;
        }
        count = count % sizeof(scancode);
    }
    return emit;
}
