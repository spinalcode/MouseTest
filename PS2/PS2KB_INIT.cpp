/**
 * PS/2 keyboard interface control class (Version 0.0.1)
 *
 * Copyright (C) 2010 Shinichiro Nakamura (CuBeatSystems)
 * http://shinta.main.jp/
 */
#include "PS2KB_INIT.h"

/**
 * Create.
 */
PS2KB_INIT::PS2KB_INIT(PinName clk_pin, PinName dat_pin)
        : clk(clk_pin), dat(dat_pin) {
    clk.input();
    dat.input();
    clk.write(1);
    dat.write(1);
    
    /*
     * 0xFF: Reset.
     * 0xED: Set/Reset status indicators.
     * 0xF2: Read ID.
     * 0xF3: Set typematic rate/delay.
     * 0xF4: Enable.
     */
    char txdat[12] = "\xFF\xED\x07\xF2\xED\x00\xF3\x20\xF3\x00\xF4";
    const int n = sizeof(txdat);
    int txerrcnt = 0;
    int rxerrcnt = 0;
    for (int i = 0; i < n; i++) {
        if (send(txdat[i]) != 0) {
            txerrcnt++;
        }
        if (recv() < 0) {
            rxerrcnt++;
        }
        if (txdat[i] == 0xF2) {
            if (recv() < 0) {
                rxerrcnt++;
            }
        }
    }
    
    if (txerrcnt > 0) {
        // printf("TX %d errors occured.\n", txerrcnt);
    }
    if (rxerrcnt > 0) {
        // printf("RX %d errors occured.\n", rxerrcnt);
    }
}

/**
 * Destroy.
 */
PS2KB_INIT::~PS2KB_INIT() {
}

/**
 * Send a byte data.
 *
 * @param c a character.
 *
 * @return Negative value is a error number.
 */
int PS2KB_INIT::send(uint8_t c) {
    clk.output();
    dat.output();

    clk.write(0);
    wait_us(200);

    dat.write(0);
    wait_us(10);
    clk.write(1);
    wait_us(10);

    clk.input();
    int parcnt = 0;
    for (int i = 0; i < 10; i++) {
        if (!waitClockDownEdge()) {
            return -1;
        }
        if ((0 <= i) && (i <= 7)) {
            /*
             * Data bit.
             */
            if ((c & (1 << i)) == 0) {
                dat.write(0);
            } else {
                dat.write(1);
                parcnt++;
            }
        }
        if (i == 8) {
            /*
             * Parity bit.
             */
            if ((parcnt % 2) == 0) {
                dat.write(1);
            } else {
                dat.write(0);
            }
        }
        if (i == 9) {
            /*
             * Stop bit.
             */
            dat.write(1);
        }
    }
    dat.input();

    /*
     * Check a ACK.
     */
    if (!waitClockDownEdge()) {
        return -2;
    }
    if (dat.read() != 0) {
        return -3;
    }

    if (!waitClockUpLevel()) {
        return -4;
    }

    return 0;
}

/**
 * Receive a byte data.
 *
 * @return return a data. Negative value is a error number.
 */
int PS2KB_INIT::recv(void) {
    uint8_t c = 0;
    clk.input();
    dat.input();
    int parcnt = 0;
    for (int i = 0; i < 11; i++) {
        if (!waitClockDownEdge()) {
            return -1;
        }
        if (i == 0) {
            /*
             * Start bit.
             */
            if (dat.read() != 0) {
                return -2;
            }
        }
        if ((1 <= i) && (i <= 8)) {
            /*
             * Data bit.
             */
            if (dat.read() == 0) {
                c &= ~(1 << (i - 1));
            } else {
                c |= (1 << (i - 1));
                parcnt++;
            }
        }
        if (i == 9) {
            /*
             * Parity bit.
             */
            if (dat.read() == 0) {
                if ((parcnt % 2) != 1) {
                    return -3;
                }
            } else {
                if ((parcnt % 2) != 0) {
                    return -4;
                }
            }
        }
        if (i == 10) {
            /*
             * Stop bit.
             */
            if (dat.read() != 1) {
                return -5;
            }
        }
    }
    return (int)c;
}

/**
 * Wait a clock down edge.
 *
 * @return true if wait done.
 */
bool PS2KB_INIT::waitClockDownEdge(void) {
    int cnt;
    /*
     * Wait until clock is low.
     */
    cnt = 0;
    while (clk.read() == 0) {
        cnt++;
        if (MAX_RETRY < cnt) {
            return false;
        }
        wait_us(1);
    }
    /*
     * Wait until clock is high.
     */
    cnt = 0;
    while (clk.read() == 1) {
        cnt++;
        if (MAX_RETRY < cnt) {
            return false;
        }
        wait_us(1);
    }
    return true;
}

/**
 * Wait a clock up level.
 *
 * @return true if wait done.
 */
bool PS2KB_INIT::waitClockUpLevel(void) {
    int cnt;
    /*
     * Wait until clock is low.
     */
    cnt = 0;
    while (clk.read() == 0) {
        cnt++;
        if (MAX_RETRY < cnt) {
            return false;
        }
        wait_us(1);
    }
    return true;
}
