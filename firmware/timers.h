#ifndef TIMER_H
#define TIMER_H

/*
 * Timer constants for prescaler 1024 at 8Mhz
 * t = (prescaler * (value + 1)) / F_CPU
 *  -> value = (t / (prescaler / F_CPU)) - 1
 *
 * with t in seconds
 */
#define TIMER1_0_1s 780
#define TIMER1_0_5s 0x0f41
#define TIMER1_1_0s 0x1e83
#define TIMER1_2_0s 0x3d08
#define TIMER1_5_0s 0x9895

void timer0_start(void);
void timer0_start_noisr(void);
uint8_t timer0_get_count(void);
void timer0_restart(uint8_t cnt);
void timer0_stop(void);

void timer1_init(uint16_t compare);
void timer1_reset(void);
void timer1_set_count(uint16_t compare);

void timer2_start(void);
uint8_t timer2_get_count(void);
void timer2_stop(void);
void timer2_restart(uint8_t cnt);

#endif
