/**
 * PS/2 mouse interface control class (Version 0.0.1)
 *
 * Copyright (C) 2010 Shinichiro Nakamura (CuBeatSystems)
 * http://shinta.main.jp/
 */
#include "PS2MS_INIT.h"

/**
 * Create.
 */
PS2MS_INIT::PS2MS_INIT(PinName clk_pin, PinName dat_pin)
        : clk(clk_pin), dat(dat_pin) {
    clk.input();
    dat.input();
    clk.write(1);
    dat.write(1);

    /*
     * 0xFF: Reset command.
     * 0xF3: Set sample rate.
     * 0xF2: Read device type.
     * 0xE8: Set resolution.
     * 0xE6: Set scaling.
     * 0xF4: Enable device.
     */
    char txdat[17] = "\xFF\xFF\xFF\xF3\xC8\xF3\x64\xF3\x50\xF2\xE8\x03\xE6\xF3\x28\xF4";
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
        if (txdat[i] == 0xFF) {
            if (recv() < 0) {
                rxerrcnt++;
            }
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
PS2MS_INIT::~PS2MS_INIT() {
}

/**
 * Send a byte data.
 *
 * @param c a character.
 *
 * @return Negative value is a error number.
 */
int PS2MS_INIT::send(uint8_t c) {
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
int PS2MS_INIT::recv(void) {
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
bool PS2MS_INIT::waitClockDownEdge(void) {
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
bool PS2MS_INIT::waitClockUpLevel(void) {
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
