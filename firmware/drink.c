#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <util/delay.h>
#include "button.h"
#include "module.h"
#include "timers.h"
#include "uart.h"
#include "led.h"

uint8_t rounds;
#define DRINKERS 6
uint8_t drinker_leds[DRINKERS] = {
    0, 2, 4, 5, 7, 9
};

uint8_t drink_round[DRINKERS];
static uint8_t drinking;

uint8_t active;
uint8_t stage;
uint8_t counter;
uint16_t timerval = 1200;

void
setup_drinkers(uint8_t forced)
{
    uint8_t drinker;
    uint8_t i;
    uint8_t chosen[DRINKERS];

    if (!forced && drinking < DRINKERS) {
        return;
    }

    memset(drink_round, 0, DRINKERS);
    memset(chosen, 0, DRINKERS);
    srand((random1 + random2) & 0xff);

    for (i = 0; i < DRINKERS; i++) {
        drinker = (rand() >> 3) & 0x7;
        if (drinker > DRINKERS) {
            drinker >>= 1;
        }

        while (chosen[drinker] == 1) {
            if (++drinker == DRINKERS) {
                drinker = 0;
            }
        }
        chosen[drinker] = 1;
        drink_round[i] = drinker;
    }
    drinking = 0;
}

void
drink_init(void)
{
    uint8_t i;

    leds_all_off();
    for (i = 0; i < DRINKERS; i++) {
        set_gradient(drinker_leds[i], 0x50, 64, 0, 32);
    }
    setup_drinkers(1);
}

void
ledmodule_drink_setup(void)
{
    uint8_t i;
    for (i = 0; i < LEDS; i++) {
        set_gradient(i, 0, 0, 0, 0);
    }

    active = 0;
    stage = 0;
    counter = 0;
    //timerval = 1200;
    timerval = 600;
}

void ledmodule_drink_isr(void)
{
    uint8_t i;
    uint8_t previous = active;

    if (stage == 0) {
        if (++active == LEDS) {
            active = 0;
            if (timerval > 1000) {
                timerval -= 200;
                timer1_set_count(timerval);
            } else if (timerval > 500) {
                timerval -= 150;
                timer1_set_count(timerval);
            } else if (timerval > 200) {
                timerval -= 50;
                timer1_set_count(timerval);
            } else {
                if (++counter == 5) {
                    counter = 0;
                    stage = 1;
                }
            }
        }
        set_output(previous, 0, 0, 0);
        set_output(active, 64, 0, 32);
    } else if (stage == 1) {
        switch (counter) {
            case 0:
                set_output(previous, 0, 0, 0);
                timer1_set_count(2000);
            case 2:
            case 4:
            case 6:
            case 8:
                for (i = 0; i < LEDS; i++) {
                    set_gradient(i, 0x20, 64, 0, 32);
                }
                break;
            case 1:
            case 3:
            case 5:
            case 7:
                for (i = 0; i < LEDS; i++) {
                    set_gradient(i, 0x20, 0, 0, 0);
                }
                break;
        }
        if (++counter == 5) {
            counter = 0;
            stage = 2;
        }
    } else if (stage == 2) {
        reset_gradients();
        for (i = 0; i < LEDS; i++) {
            set_output(i, 0, 0, 0);
        }
        set_gradient(drinker_leds[drink_round[drinking++]], 0x20, 64, 0, 0);
        set_gradient(drinker_leds[drink_round[drinking++]], 0x20, 0, 0, 64);
        stage = 3;
        timer2_start();
    } else {
        /* do nothing */
    }

}

ledmodule_t ledmodule_drink = {
    .clock = 1400,
    .setup = ledmodule_drink_setup,
    .isr = ledmodule_drink_isr
};

