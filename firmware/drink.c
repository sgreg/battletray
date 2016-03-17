#include <stdint.h>
#include <util/delay.h>
#include "button.h"
#include "uart.h"
#include "led.h"

void
print_randoms(void)
{
    uart_puthex(random1);
    uart_putchar(' ');
    uart_puthex(random2);
    uart_putchar(' ');
    uart_puthex(random3);
    uart_putchar(' ');
    uart_puthex(random4);
    uart_newline();
}


uint8_t rounds;
#define DRINKERS 12
uint8_t have_drunk;
uint8_t drunk[DRINKERS];
uint8_t reverse_drunk[DRINKERS];
uint8_t lookup[DRINKERS];
void show_stats(void);

static uint8_t
get_idx(uint8_t num)
{
    if (num >= DRINKERS) {
        num = num >> 1;
    }
    return num;
}

void
you_drink(void)
{
    uint8_t first;
    uint8_t second;
    uint8_t i;

    rounds++;
    do {
        first = get_idx((random2 & 0xf0) >> 4);
        second = get_idx(random2 & 0x0f);
        //random2 = TCNT2; get it from button_* somehow
    } while (first == second);

    if ((rounds & 0x3) == 0) {
    }

    drunk[first]++;
    drunk[second]++;
    uart_print("DRINK!\r\n");
    uart_putint(first + 1, 1);
    uart_print(" vs ");
    uart_putint(second + 1, 1);
    uart_newline();

    for (i = 0; i < DRINKERS; i++) {
        reverse_drunk[drunk[i]]++;
    }

    if ((rounds & 0xf) == 0) {
        show_stats();
    }
}

void
show_stats(void)
{
    uint8_t i;

    uart_print("\r\nstats after ");
    uart_putint(rounds, 1);
    uart_print(" rounds\r\n");
    for(i = 0; i < DRINKERS; i++) {
        uart_putint(i, 2);
        uart_print(": ");
        uart_putint(drunk[i], 2);
        uart_print("  rd ");
        uart_putint(reverse_drunk[i], 2);
        uart_newline();
    }
}

void
run_drink(void)
{
    led_off(2);
    led_on(0);
    rgb_set(0, 0, 1);
    _delay_us(5);
    rgb_set(1, 0, 0);
    _delay_us(25);
    rgb_set(0, 0, 0);
    _delay_us(20);

    led_off(0);
    led_on(1);
    rgb_set(0, 0, 1);
    _delay_us(20);
    rgb_set(0, 1, 0);
    _delay_us(25);
    rgb_set(1, 0, 0);
    _delay_us(5);

    led_off(1);
    led_on(2);
    rgb_set(1, 0, 0);
    _delay_us(45);
    rgb_set(0, 1, 0);
    _delay_us(5);
}

