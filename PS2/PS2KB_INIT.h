/**
 * PS/2 keyboard interface control class (Version 0.0.1)
 *
 * Copyright (C) 2010 Shinichiro Nakamura (CuBeatSystems)
 * http://shinta.main.jp/
 */
#ifndef _PS2KB_INIT_H_
#define _PS2KB_INIT_H_

#include "mbed.h"

/**
 * PS2 keyboard initializer.
 */
class PS2KB_INIT {
public:

    /**
     * Create.
     */
    PS2KB_INIT(PinName clk_pin, PinName dat_pin);

    /**
     * Destroy.
     */
    ~PS2KB_INIT();
private:
    DigitalInOut clk;
    DigitalInOut dat;

    static const int MAX_RETRY = 1000000;

    /**
     * Send a byte data.
     *
     * @param c a character.
     *
     * @return Negative value is a error number.
     */
    int send(uint8_t c);

    /**
     * Receive a byte data.
     *
     * @return return a data. Negative value is a error number.
     */
    int recv(void);

    /**
     * Wait a clock down edge.
     *
     * @return true if wait done.
     */
    bool waitClockDownEdge(void);

    /**
     * Wait a clock up level.
     *
     * @return true if wait done.
     */
    bool waitClockUpLevel(void);
};

#endif
