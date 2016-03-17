#include <stdint.h>
#include "module.h"
#include "timers.h"
#include "led.h"


static int8_t rainbow_rgb[3];
static uint8_t rainbow_state;
static uint8_t rainbow_cnt;


void
ledmodule_rainbow_setup(void)
{
    uint8_t i;
    for (i = 0; i < LEDS; i++) {
        set_output(i, 64, 0, 0);
    }
}

void ledmodule_rainbow_isr(void)
{
    uint8_t i;

    if (++rainbow_cnt == 64) {
        if (++rainbow_state == 6) {
            rainbow_state = 0;
        }
        rainbow_cnt = 0;
    }

    switch (rainbow_state) {
        case 0:
            rainbow_rgb[0] = 64;
            rainbow_rgb[1] =  0;
            rainbow_rgb[2] = rainbow_cnt;
            break;
        case 1:
            rainbow_rgb[0] = 64 - rainbow_cnt;
            rainbow_rgb[1] =  0;
            rainbow_rgb[2] = 64;
            break;
        case 2:
            rainbow_rgb[0] = 0;
            rainbow_rgb[1] = rainbow_cnt;
            rainbow_rgb[2] = 64;
            break;
        case 3:
            rainbow_rgb[0] = 0;
            rainbow_rgb[1] = 64;
            rainbow_rgb[2] = 64 - rainbow_cnt;
            break;
        case 4:
            rainbow_rgb[0] = rainbow_cnt;
            rainbow_rgb[1] = 64;
            rainbow_rgb[2] = 0;
            break;
        case 5:
            rainbow_rgb[0] = 64;
            rainbow_rgb[1] = 64 - rainbow_cnt;
            rainbow_rgb[2] = 0;
            break;
    }

    for (i = 0; i < LEDS; i++) {
        set_output(i, rainbow_rgb[0], rainbow_rgb[1], rainbow_rgb[2]);
    }
}

ledmodule_t ledmodule_rainbow = {
    .clock = 0x17d7, /* ~781ms, 6 states of 64 counts each == 5min */
    .setup = ledmodule_rainbow_setup,
    .isr = ledmodule_rainbow_isr
};

