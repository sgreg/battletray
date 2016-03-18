#ifndef LED_H
#define LED_H
#include <stdint.h>

#define RED 0
#define GREEN 1
#define BLUE 2

#define LEDS 10

void rgb_on(uint8_t color);
void rgb_off(uint8_t color);

#define rgb_red_on()        rgb_on(RED)
#define rgb_red_off()       rgb_off(RED)
#define rgb_green_on()      rgb_on(GREEN)
#define rgb_green_off()     rgb_off(GREEN)
#define rgb_blue_on()       rgb_on(BLUE)
#define rgb_blue_off()      rgb_off(BLUE)


void setup_rgb(uint8_t color, volatile uint8_t *ddr, volatile uint8_t *port, uint8_t pin);
void setup_led(uint8_t num, volatile uint8_t *ddr, volatile uint8_t *port, uint8_t pin);

void rgb_set(uint8_t red, uint8_t green, uint8_t blue);

void led_on(uint8_t num);
void led_off(uint8_t num);

void leds_all_off(void);

void set_gradient(uint8_t led, uint16_t count, uint8_t r, uint8_t g, uint8_t b);
void reset_gradients(void);

void set_output(uint8_t led, uint8_t r, uint8_t g, uint8_t b);

void run_light(void);
#endif

