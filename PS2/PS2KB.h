/**
 * PS/2 keyboard interface control class (Version 0.0.1)
 *
 * Copyright (C) 2010 Shinichiro Nakamura (CuBeatSystems)
 * http://shinta.main.jp/
 */

#ifndef _PS2KB_H_
#define _PS2KB_H_

#include "mbed.h"

/**
 * PS/2 keyboard interface control class.
 */
class PS2KB {
public:
    /**
     * Create.
     *
     * @param clk_pin Clock pin.
     * @param dat_pin Data pin.
     */
    PS2KB(PinName clk_pin, PinName dat_pin);

    /**
     * Destory.
     */
    virtual ~PS2KB();

    /**
     * Get a data from a PS/2 device.
     *
     * @return A data from a PS/2 device.
     */
    virtual int getc(void);

    /**
     * Set timeout.
     *
     * @param ms Timeout ms.
     */
    virtual void setTimeout(int ms);

private:
    static const int RINGBUFSIZ = 256;
    InterruptIn clk;    /**< Interrupt input for CLK. */
    DigitalIn dat;      /**< Digital input for DAT. */
    Timeout wdt;    /**< Watch dog timer. */
    Timer tot;      /**< Timeout timer. */
    int timeout;    /**< Timeout[ms] for getc(). */

    typedef struct {
        int bitcnt;
        int cStart;
        int cEnd;
        uint8_t buffer[RINGBUFSIZ];
    } work_t;
    work_t work;

    void func_timeout(void);
    void func_fall(void);

    void init_work(void);
};

#endif
