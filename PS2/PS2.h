/**
 * PS/2 interface control class (Version 0.0.1)
 *
 * Copyright (C) 2010 Shinichiro Nakamura (CuBeatSystems)
 * http://shinta.main.jp/
 */

#ifndef _PS2_H_
#define _PS2_H_

#include "mbed.h"

/**
 * PS/2 interface control class.
 */
class PS2 {
public:
    /**
     * Create.
     *
     * @param clk_pin Clock pin.
     * @param dat_pin Data pin.
     */
    PS2(PinName clk_pin, PinName dat_pin);

    /**
     * Destory.
     */
    virtual ~PS2();

    /**
     * Get a data from a PS/2 device.
     *
     * @return A data from a PS/2 device.
     */
    virtual int getc(void) = 0;

    /**
     * Set timeout.
     *
     * @param ms Timeout ms.
     */
    virtual void setTimeout(int ms) = 0;
};

#endif
