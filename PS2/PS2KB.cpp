/**
 * PS/2 keyboard interface control class (Version 0.0.1)
 *
 * Copyright (C) 2010 Shinichiro Nakamura (CuBeatSystems)
 * http://shinta.main.jp/
 */

#include "PS2KB.h"

/**
 * Create.
 *
 * @param clk_pin Clock pin.
 * @param dat_pin Data pin.
 */
PS2KB::PS2KB(PinName clk_pin, PinName dat_pin)
        : clk(clk_pin), dat(dat_pin) {
    init_work();
    clk.fall(this, &PS2KB::func_fall);
    timeout = 1;
}

/**
 * Destory.
 */
PS2KB::~PS2KB() {
    wdt.detach();
}

/**
 * Get a data from a PS/2 device.
 *
 * @return A data from a PS/2 device.
 */
int PS2KB::getc() {
    tot.reset();
    tot.start();
    while (work.cStart == work.cEnd) {
        wait_ms(1);
        if ((timeout > 0) && (tot.read_ms() > timeout)) {
            // printf("Timeout occured.\n");
            return EOF;
        }
    }
    tot.stop();

    char c = work.buffer[work.cStart++];
    work.cStart =  work.cStart % RINGBUFSIZ;

    return c;
}

/**
 * Set timeout.
 *
 * @param ms Timeout ms.
 */
void PS2KB::setTimeout(int ms) {
    timeout = ms;
}

void PS2KB::func_timeout(void) {
    work.bitcnt = 0;
}

void PS2KB::func_fall(void) {
    int oddpar = 0;
    /*
     */
    switch (work.bitcnt) {
        case 0:
            /*
             * Start bit.
             */
            if (dat.read() != 0) {
                // printf("Illegal start bit condition.\n");
            }
            work.bitcnt++;
            break;
        case 9:
            /*
             * Parity bit.
             */
            for (int i = 0; i < 8; i++) {
                if ((work.buffer[work.cEnd] & (1 << i)) != 0) {
                    oddpar++;
                }
            }
            if (dat.read() == 1) {
                oddpar++;
            }
            if ((oddpar % 2) != 1) {
                // printf("Data parity error.\n");
            }
            work.bitcnt++;
            break;
        case 10:
            /*
             * Stop bit.
             */
            if (dat.read() != 1) {
                // printf("Illegal stop bit condition.\n");
            }
            if (work.cStart != ((work.cEnd + 1) % RINGBUFSIZ)) {
                work.cEnd++;
                work.cEnd = work.cEnd % RINGBUFSIZ;
                work.bitcnt = 0;
            } else {
                // printf("Buffer overrun.\n");
            }
            break;
        default:
            if ((1 <= work.bitcnt) && (work.bitcnt <= 8)) {
                /*
                 * data bit.
                 */
                if (dat.read() == 1) {
                    work.buffer[work.cEnd] |= (1 << (work.bitcnt - 1));
                } else {
                    work.buffer[work.cEnd] &= ~(1 << (work.bitcnt - 1));
                }
                work.bitcnt++;
            } else {
                /*
                 * Illegal internal state.
                 */
                // printf("Illegal internal state found.\n");
                init_work();
            }
            break;
    }
    wdt.detach();
    wdt.attach_us(this, &PS2KB::func_timeout, 250);
}

void PS2KB::init_work(void) {
    work.bitcnt = 0;
    work.cStart = 0;
    work.cEnd = 0;
}
