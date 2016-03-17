#include <stdint.h>
#include "module.h"
#include "timers.h"
#include "led.h"


static int8_t rgb[LEDS][3];
static uint8_t states[LEDS];

void
ledmodule_rainbow2_setup(void)
{
    uint8_t i;

    /* XXX if LEDS value changes, this has to be adjusted manually */
    rgb[0][0] = 64;
    rgb[0][1] =  0;
    rgb[0][2] =  0;
    states[0] =  0;

    rgb[1][0] = 64;
    rgb[1][1] =  0;
    rgb[1][2] = 38;
    states[1] =  0;

    rgb[2][0] = 51;
    rgb[2][1] =  0;
    rgb[2][2] = 64;
    states[2] =  1;

    rgb[3][0] = 13;
    rgb[3][1] =  0;
    rgb[3][2] = 64;
    states[3] =  1;

    rgb[4][0] =  0;
    rgb[4][1] = 25;
    rgb[4][2] = 64;
    states[4] =  2;

    rgb[5][0] =  0;
    rgb[5][1] = 64;
    rgb[5][2] = 64;
    states[5] =  3;

    rgb[6][0] =  0;
    rgb[6][1] = 64;
    rgb[6][2] = 26;
    states[6] =  3;

    rgb[7][0] = 13;
    rgb[7][1] = 64;
    rgb[7][2] =  0;
    states[7] =  4;

    rgb[8][0] = 51;
    rgb[8][1] = 64;
    rgb[8][2] =  0;
    states[8] =  4;

    rgb[9][0] = 64;
    rgb[9][1] = 39;
    rgb[9][2] =  0;
    states[9] =  5;

    for (i = 0; i < LEDS; i++) {
        set_gradient(i, 0x80, rgb[i][0], rgb[i][1], rgb[i][2]);
    }
}

void ledmodule_rainbow2_isr(void)
{
    uint8_t i;

    for (i = 0; i < LEDS; i++) {

        switch (states[i]) {
            case 0:
                rgb[i][0] = 64;
                rgb[i][1] =  0;
                rgb[i][2]++;
                if (rgb[i][2] == 64) {
                    states[i] = 1;
                }
                break;
            case 1:
                rgb[i][0]--;
                rgb[i][1] =  0;
                rgb[i][2] = 64;
                if (rgb[i][0] == 0) {
                    states[i] = 2;
                }
                break;
            case 2:
                rgb[i][0] = 0;
                rgb[i][1]++;
                rgb[i][2] = 64;
                if (rgb[i][1] == 64) {
                    states[i] = 3;
                }
                break;
            case 3:
                rgb[i][0] = 0;
                rgb[i][1] = 64;
                rgb[i][2]--;
                if (rgb[i][2] == 0) {
                    states[i] = 4;
                }
                break;
            case 4:
                rgb[i][0]++;
                rgb[i][1] = 64;
                rgb[i][2] = 0;
                if (rgb[i][0] == 64) {
                    states[i] = 5;
                }
                break;
            case 5:
                rgb[i][0] = 64;
                rgb[i][1]--;
                rgb[i][2] = 0;
                if (rgb[i][1] == 0) {
                    states[i] = 0;
                }
                break;
        }

        set_output(i, rgb[i][0], rgb[i][1], rgb[i][2]);
    }
}

ledmodule_t ledmodule_rainbow2 = {
    .clock = 0x17d7, /* ~781ms, 6 states of 64 counts each == 5min */
    //.clock = TIMER1_0_1s,
    .setup = ledmodule_rainbow2_setup,
    .isr = ledmodule_rainbow2_isr
};


