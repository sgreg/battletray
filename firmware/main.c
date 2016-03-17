/*
 *
 *
 * ATmega48 pin layout (PDIP)
 *
 *   1  /Reset
 *   2  PD0     I   UART RXD
 *   3  PD1     O   UART TXD
 *   4  PD2     I   INT0 button
 *   6  PD3     -
 *   5  PD4     -
 *   7  VCC     -
 *   8  GND     -
 *   9  PB6     -
 *  10  PB7     O   LED2
 *  11  PD5     O   LED1
 *  12  PD6     O   LED0
 *  13  PD7     O   LED9
 *  14  PB0     O   LED8
 *
 *  15  PB1     O   LED blue
 *  16  PB2     O   LED green
 *  17  PB3     O   LED red / SerProg MOSI
 *  18  PB4     I   SerProg MISO
 *  19  PB5     O   SerProg SCK
 *  20  AVCC    -
 *  21  AREF    -
 *  22  GND     -
 *  23  PC0     -
 *  24  PC1     O   LED7
 *  25  PC2     O   LED6
 *  26  PC3     O   LED5
 *  27  PC4     O   LED4
 *  28  PC5     O   LED3
 *
 *
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <string.h>
#include "uart.h"

#include "button.h"
#include "timers.h"
#include "drink.h"
#include "led.h"
#include "mode.h"
#include "module.h"


uint8_t mode = MODE_LIGHT;
uint8_t submode;

int
main(void)
{
    // pullup on button pin
    PORTD = (1 << PD2);

    EICRA = 0x01; // falling edge interrupt enable on int0
    EIMSK = 0x01; // enable int0 interrupt

    // TODO: add setup_button(...) function to button.c

    setup_rgb(RED,   &DDRB, &PORTB, 3);
    setup_rgb(GREEN, &DDRB, &PORTB, 2);
    setup_rgb(BLUE,  &DDRB, &PORTB, 1);

    setup_led(7, &DDRC, &PORTC, 1); // pin 24
    setup_led(6, &DDRC, &PORTC, 2); // pin 25
    setup_led(5, &DDRC, &PORTC, 3); // pin 26
    setup_led(4, &DDRC, &PORTC, 4); // pin 27
    setup_led(3, &DDRC, &PORTC, 5); // pin 28
    setup_led(2, &DDRB, &PORTB, 7); // pin 10
    setup_led(1, &DDRD, &PORTD, 5); // pin 11
    setup_led(0, &DDRD, &PORTD, 6); // pin 12
    setup_led(9, &DDRD, &PORTD, 7); // pin 13
    setup_led(8, &DDRB, &PORTB, 0); // pin 14

    uart_init(UART_BRATE_9600_8MHZ);
    sei();

    uart_print("yo!\r\n\r\n");

    /* default start with first ledmodule */
    ledmodule_activate(0);


    while (1) {
        if (button_longpress) {
            mode = (mode == MODE_LIGHT) ? MODE_DRINK : MODE_LIGHT;
            submode = 0;
            button_longpress = 0;
            button_press = 0;

        } else if (button_press) {
            if (mode == MODE_LIGHT) {
                ledmodule_deactivate(submode);
                if (++submode == SUBMODE_LIGHT_MAX) {
                    submode = 0;
                }
                uart_print("\r\nmode ");
                uart_puthex(submode);
                ledmodule_activate(submode);
            } else {
                print_randoms();
                you_drink();
            }

            button_press = 0;

        } else {
            if (mode == MODE_LIGHT) {
                run_light();
            } else {
                run_drink();
            }
        }
    }

    return 0;
}

