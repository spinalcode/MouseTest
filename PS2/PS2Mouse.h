/**
 * PS/2 mouse interface control class (Version 0.0.1)
 *
 * Copyright (C) 2010 Shinichiro Nakamura (CuBeatSystems)
 * http://shinta.main.jp/
 */
#ifndef _PS2_MOUSE_H_
#define _PS2_MOUSE_H_

#include "PS2MS_INIT.h"
#include "PS2MS.h"

class PS2Mouse {
public:
    PS2Mouse(PinName clk_pin, PinName dat_pin);
    ~PS2Mouse();
    typedef struct {
        bool left;
        bool center;
        bool right;
        int x;
        int y;
        int z;
    } mouse_event_t;
    bool processing(mouse_event_t *p);
private:
    PS2MS_INIT ps2ms_init;
    PS2MS ps2ms;
    typedef struct {
        union {
            uint8_t byte;
            struct {
                uint8_t btnLeft:1;
                uint8_t btnRight:1;
                uint8_t btnCenter:1;
                uint8_t always1:1;
                uint8_t signX:1;
                uint8_t signY:1;
                uint8_t overflowX:1;
                uint8_t overflowY:1;
            } bit;
        } byte1;
        union {
            uint8_t byte;
        } byte2;
        union {
            uint8_t byte;
        } byte3;
        union {
            uint8_t byte;
            struct {
                uint8_t value:7;
                uint8_t signZ:1;
            } bit;
        } byte4;
    } mouse_info_t;
    mouse_info_t mi;
    int cnt;
};

#endif
