#include <stdint.h>
#include "module.h"
#include "timers.h"
#include "led.h"

uint8_t active;
uint8_t stage;
uint8_t counter;
uint16_t timerval = 1200;

void
ledmodule_round_setup(void)
{
    uint8_t i;
    for (i = 0; i < LEDS; i++) {
        set_gradient(i, 0, 0, 0, 0);
    }

    active = 0;
    stage = 0;
    counter = 0;
    timerval = 1200;
}

void ledmodule_round_isr(void)
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
        set_gradient(2, 0x20, 64, 0, 0);
        set_gradient(4, 0x20, 0, 0, 64);
        stage = 3;
    } else {
        /* do nothing */
    }

}

ledmodule_t ledmodule_round = {
    .clock = 1400,
    .setup = ledmodule_round_setup,
    .isr = ledmodule_round_isr
};

