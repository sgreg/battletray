#include <stdint.h>
#include "module.h"
#include "timers.h"
#include "led.h"

static uint8_t base[] = {56, 0, 32};
static uint8_t changed[LEDS];
static uint8_t nextled;

void
ledmodule_purple_setup(void)
{
    uint8_t i;
    for (i = 0; i < LEDS; i++) {
        set_gradient(i, 0x7f, base[0], base[1], base[2]);
    }
}

void ledmodule_purple_isr(void)
{
    uint8_t i;
    uint8_t val1 = timer0_get_count();
    uint8_t val2 = timer2_get_count();
    uint8_t red, blue;

    uint8_t offset = ((val1 >> 4) & 0x07) + 1;
    uint8_t r = val2 & 0x03;
    uint8_t b = (val2 & 0x0c) >> 2;

    for (i = 0; i < LEDS; i++) {
        if (changed[i] && (val1 & 0x01)) {
            set_gradient(i, 0x50 + (val1 & 0x3f), base[0], base[1], base[2]);
            changed[i] = 0;
            if (val1 & 0x08) {
                break;
            }
        }
    }

    nextled += offset;
    if (nextled >= LEDS) {
        nextled -= LEDS;
    }

    red = base[0];
    if (r == 0x01) {
        red += 8;
    } else if (r == 0x02) {
        red -= 8;
    }

    blue = base[2];
    if (red != base[0]) {
        if (b == 0x02) {
            blue -= 16;
        } else if (b == 0x01) {
            blue += 8;
        }
    } else {
        if (b == 0x01) {
            blue += 8;
        } else {
            blue -= b * 8;
        }
    }

    changed[nextled] = 1;

    set_gradient(nextled, 0x7f + (val1 & 0x3f), red, 0, blue); // XXX maybe a little bit of green here and there?
    timer2_restart(val1 + red + b);
    timer0_restart(val2 + blue + r);
}

ledmodule_t ledmodule_purple = {
    .clock = TIMER1_0_5s,
    .setup = ledmodule_purple_setup,
    .isr = ledmodule_purple_isr
};
