#include <stdint.h>
#include <avr/io.h>
#include "module.h"
#include "timers.h"
#include "led.h"
#include "uart.h"


static uint8_t nextled;

void
ledmodule_random_setup(void)
{
    timer0_start_noisr();
    timer2_start();
}

void
ledmodule_random_teardown(void)
{
    timer0_stop();
    timer2_stop();
}

void ledmodule_random_isr(void)
{
    uint8_t val1 = timer0_get_count();
    uint8_t val2 = timer2_get_count();

    uint8_t offset = ((val1 >> 4) & 0x07) + 1;
    uint8_t r = (val1 & 0x0f) << 2;
    uint8_t g = (val2 & 0xf0) >> 2;
    uint8_t b = (val2 & 0x0f) << 2;

    nextled += offset;
    if (nextled >= LEDS) {
        nextled -= LEDS;
    }

    /*
    uart_print("set ");
    uart_puthex(nextled);
    uart_print(" to ");
    uart_puthex(r);
    uart_putchar(' ');
    uart_puthex(g);
    uart_putchar(' ');
    uart_puthex(b);
    uart_newline();
    */

    set_gradient(nextled, 0xff, r, g, b);

    timer2_restart(val1 + r + b);
    timer0_restart(val2 + g + nextled);
}

ledmodule_t ledmodule_random = {
    .clock = TIMER1_2_0s,
    .setup = ledmodule_random_setup,
    .teardown = ledmodule_random_teardown,
    .isr = ledmodule_random_isr
};


