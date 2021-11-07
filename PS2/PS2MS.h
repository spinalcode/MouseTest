/**
 * PS/2 mouse interface control class (Version 0.0.1)
 *
 * Copyright (C) 2010 Shinichiro Nakamura (CuBeatSystems)
 * http://shinta.main.jp/
 */

#ifndef _PS2MS_H_
#define _PS2MS_H_

#include "mbed.h"

/**
 * PS/2 mouse interface control class.
 */
class PS2MS {
public:
    /**
     * Create.
     *
     * @param clk_pin Clock pin.
     * @param dat_pin Data pin.
     */
    PS2MS(PinName clk_pin, PinName dat_pin);

    /**
     * Destory.
     */
    virtual ~PS2MS();

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
    static const int RINGBUFSIZ = 1024;
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
