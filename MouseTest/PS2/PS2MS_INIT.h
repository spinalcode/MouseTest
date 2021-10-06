/**
 * PS/2 mouse interface control class (Version 0.0.1)
 *
 * Copyright (C) 2010 Shinichiro Nakamura (CuBeatSystems)
 * http://shinta.main.jp/
 */
#ifndef _PS2MS_INIT_H_
#define _PS2MS_INIT_H_

#include "mbed.h"

/**
 * PS2 mouse initializer.
 */
class PS2MS_INIT {
public:

    /**
     * Create.
     */
    PS2MS_INIT(PinName clk_pin, PinName dat_pin);

    /**
     * Destroy.
     */
    ~PS2MS_INIT();
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
