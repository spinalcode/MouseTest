/**
 * PS/2 mouse interface control class (Version 0.0.1)
 *
 * Copyright (C) 2010 Shinichiro Nakamura (CuBeatSystems)
 * http://shinta.main.jp/
 */
#include "PS2Mouse.h"

PS2Mouse::PS2Mouse(PinName clk_pin, PinName dat_pin)
        : ps2ms_init(clk_pin, dat_pin), ps2ms(clk_pin, dat_pin) {
    cnt = 0;
}

PS2Mouse::~PS2Mouse() {
}

bool PS2Mouse::processing(mouse_event_t *p) {
    bool emit = false;
    for (int i = 0; i < 4; i++) {
        const int c = ps2ms.getc();
        if (0 <= c) {
            switch (cnt % 4) {
                case 0:
                    mi.byte1.byte = c;
                    /*
                     * Check and reset a buffer if state is wrong.
                     */
                    if (mi.byte1.bit.always1 == 0) {
                        cnt = 0;
                        while (0 <= ps2ms.getc()) {
                        }
                    }
                    break;
                case 1:
                    mi.byte2.byte = c;
                    break;
                case 2:
                    mi.byte3.byte = c;
                    break;
                case 3:
                    mi.byte4.byte = c;
                    /*
                     * Store a event data.
                     */
                    p->left = mi.byte1.bit.btnLeft ? true : false;
                    p->center = mi.byte1.bit.btnCenter ? true : false;
                    p->right = mi.byte1.bit.btnRight ? true : false;
                    p->x = mi.byte1.bit.signX ? (-256 + mi.byte2.byte) : mi.byte2.byte;
                    p->y = mi.byte1.bit.signY ? (-256 + mi.byte3.byte) : mi.byte3.byte;
                    p->z = mi.byte4.bit.signZ ? (-128 + mi.byte4.bit.value) : mi.byte4.bit.value;
                    emit = true;
                    break;
            }
            cnt++;
        }
    }
    return emit;
}
