#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "button.h"
#include "module.h"
#include "mode.h"

/*
 * Light run mode timer
 */
void
timer1_init(uint16_t compare)
{
    TCCR1A = 0x00;
    TCCR1B = 0x0d; /* CTC, Prescaler 1024 */
    OCR1A  = compare;
    TCNT1 = 0x0000;
    TIMSK1 = 0x02; /* counter a match interrupt enable */
}

void
timer1_reset(void)
{
    TCCR1B = 0x00; /* clock off */
    OCR1A = 0x0000;
    TCNT1 = 0x0000;
}

void
timer1_set_count(uint16_t compare)
{
    OCR1A = compare;
}

/*
 *  TIMER0  -  Button mode change timer
 */

void
timer0_start(void)
{
    TCCR0A = 0x00; /* Normal op, no output modification */
    TCCR0B = 0x05; /* prescaler 1024 */
    TCNT0 = 0x00;
    TIMSK0 = 0x01; /* overflow interrupt enable */
}

void
timer0_start_noisr(void)
{
    TCCR0A = 0x00; /* Normal op, no output modification */
    TCCR0B = 0x03; /* prescaler 64 */
    TCNT0 = 0x00;
}

uint8_t
timer0_get_count(void)
{
    return TCNT0;
}

void
timer0_restart(uint8_t cnt)
{
    TCNT0 = cnt;
}

void
timer0_stop(void)
{
    TCCR0B = 0x00;
    TIMSK0 = 0x00;
    random4 = TCNT0;
}

ISR(TIMER0_OVF_vect)
{
    button_poll();
}

ISR(TIMER1_COMPA_vect)
{
    ledmodule_t *mod;

    if (mode == MODE_LIGHT) {
        if ( (mod = ledmodules[submode]) != NULL && mod->isr) {
            mod->isr();
        }
    }
}


/*
 *  TIMER2  - Aux timer
 */
void
timer2_start(void)
{
    TCCR2A = 0x00; /* Normal op, no output modification */
    TCCR2B = 0x02; /* prescaler 8 */
}

uint8_t
timer2_get_count(void)
{
    return TCNT2;
}

void
timer2_stop(void)
{
    TCCR2B = 0x00;
}

void
timer2_restart(uint8_t cnt)
{
    TCNT2 = cnt;
}

