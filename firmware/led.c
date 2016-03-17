#include <string.h>
#include <util/delay.h>
#include "led.h"

struct led {
    volatile uint8_t *port;
    uint8_t pin;
};

static struct led leds[LEDS];


static struct led rgb[3];


void
rgb_on(uint8_t color)
{
    *rgb[color].port |= (1 << rgb[color].pin);
}

void
rgb_off(uint8_t color)
{
    *rgb[color].port &= ~(1 << rgb[color].pin);
}

void
rgb_set(uint8_t red, uint8_t green, uint8_t blue)
{
    (red)   ? rgb_on(RED)   : rgb_off(RED);
    (green) ? rgb_on(GREEN) : rgb_off(GREEN);
    (blue)  ? rgb_on(BLUE)  : rgb_off(BLUE);
}




void
led_on(uint8_t num)
{
    if (num < LEDS) {
        *leds[num].port |= (1 << leds[num].pin);
    }
}

void
led_off(uint8_t num)
{
    if (num < LEDS) {
        *leds[num].port &= ~(1 << leds[num].pin);
    }
}

void
setup_led(uint8_t num, volatile uint8_t *ddr, volatile uint8_t *port, uint8_t pin)
{
    if (num < LEDS) {
        leds[num].port = port;
        leds[num].pin  = pin;
        *ddr |= (1 << pin);
    }
}
void
setup_rgb(uint8_t color, volatile uint8_t *ddr, volatile uint8_t *port, uint8_t pin)
{
    // XXX this is same than setup_led
    rgb[color].port = port;
    rgb[color].pin  = pin;
    *ddr |= (1 << pin);
}


struct gradient {
    uint8_t enable;
    uint16_t maxcount;
    uint16_t counter;
    uint8_t rgb[3];
};

struct gradient gradients[LEDS + 10];

/*
static uint8_t
gradient_ongoing(uint8_t led)
{
    return gradients[led].enable;
}
*/


void
set_gradient(uint8_t led, uint16_t count, uint8_t r, uint8_t g, uint8_t b)
{
    gradients[led].counter = 0;
    gradients[led].maxcount = count;
    gradients[led].rgb[0] = r;
    gradients[led].rgb[1] = g;
    gradients[led].rgb[2] = b;
    gradients[led].enable = 1;
}

void
reset_gradients(void)
{
    memset(gradients, 0x00, sizeof(gradients));
}


uint8_t output[LEDS][3];

void
set_output(uint8_t led, uint8_t r, uint8_t g, uint8_t b)
{
    output[led][0] = r;
    output[led][1] = g;
    output[led][2] = b;
}

void
run_light(void)
{
    uint8_t i;
    uint8_t j;
    uint8_t c;
    int16_t diff[3];
    struct gradient *g;

    for (i = 0; i < LEDS; i++) {
        if (output[i][0] > 0 || output[i][1] > 0 || output[i][2] > 0) {
            led_on(i);
        }
        for (c = 0; c < 64; c++) {
            (output[i][0] > c) ? rgb_on(RED)   : rgb_off(RED);
            (output[i][1] > c) ? rgb_on(GREEN) : rgb_off(GREEN);
            (output[i][2] > c) ? rgb_on(BLUE)  : rgb_off(BLUE);

            g = &gradients[i];
            if (g->enable) {
                if (++g->counter < g->maxcount) {
                    continue;
                }
                g->counter = 0;

                for (j = 0; j < 3; j++) {
                    /* for each LED calculate difference */
                    diff[j] = g->rgb[j] - output[i][j];
                    /* increase/decrease output while there's a diff */
                    if (diff[j] > 0) {
                        output[i][j]++;
                    } else if (diff[j] < 0) {
                        output[i][j]--;
                    }
                }
                /* if all diffs are zero, disable gradient */
                if (diff[0] == 0 && diff[1] == 0 && diff[2] == 0) {
                    g->enable = 0;
                    continue;
                }
            }
        }
        led_off(i);
        //_delay_us(100);
    }
}

