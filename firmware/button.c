#include <avr/io.h>
#include <avr/interrupt.h>
#include "timers.h"

uint8_t button_press;
uint8_t button_longpress;

uint8_t random1;
uint8_t random2;
uint8_t random3;
uint8_t random4;

static uint8_t counter;
#define MAX_COUNT 40

static uint8_t
button_pressed(void)
{
    return ((PIND & (1 << PD2)) == 0);
}

void
button_poll(void)
{
    if (button_pressed()) {
        if (++counter == MAX_COUNT) {
            button_longpress = 1;
            counter = 0;
            timer0_stop();
        }
    } else {
        button_press = 1;
        counter = 0;
        timer0_stop();
        random4 = TCNT2;
    }
}

ISR(INT0_vect)
{
    if ((PIND & (1 << PD2)) == 0) {
        /* button press */
        counter = 0;
        timer0_start();
        random1 = TCNT2;

    } else {
        /* button release */
        random2 = TCNT2;
        random3 = TCNT0;
    }
}

