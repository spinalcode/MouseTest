/**
 * PS/2 keyboard interface control class (Version 0.0.1)
 *
 * Copyright (C) 2010 Shinichiro Nakamura (CuBeatSystems)
 * http://shinta.main.jp/
 */
#ifndef _PS2_KEYBOARD_H_
#define _PS2_KEYBOARD_H_

#include "PS2KB_INIT.h"
#include "PS2KB.h"

class PS2Keyboard {
public:
    PS2Keyboard(PinName clk_pin, PinName dat_pin);
    ~PS2Keyboard();
    typedef enum {
        KeyMake,
        KeyBreak
    } Type;
    typedef struct {
        Type type;
        int length;
        char scancode[8];
    } keyboard_event_t;
    bool processing(keyboard_event_t *p);
private:
    PS2KB_INIT ps2kb_init;
    PS2KB ps2kb;
    int count;
    char scancode[8];
};

#endif
