#include <stdint.h>
#include "module.h"
#include "led.h"
#include "timers.h"

extern ledmodule_t ledmodule_purple;
extern ledmodule_t ledmodule_rainbow;
extern ledmodule_t ledmodule_rainbow2;
extern ledmodule_t ledmodule_random;
extern ledmodule_t ledmodule_drink;


ledmodule_t *ledmodules[] = {
    &ledmodule_purple,
    &ledmodule_rainbow,
    &ledmodule_rainbow2,
    &ledmodule_random,
    /* drink! */
    &ledmodule_drink
};

uint8_t ledmodule_count = sizeof(ledmodules);


void
ledmodule_activate(uint8_t index)
{
    timer1_reset();
    ledmodule_t *mod = ledmodules[index];
    reset_gradients();
    rgb_set(0, 0, 0);
    timer1_init(mod->clock);
    if (mod->setup) {
        mod->setup();
    }
}

void
ledmodule_deactivate(uint8_t index)
{
    ledmodule_t *mod = ledmodules[index];
    if (mod->teardown) {
        mod->teardown();
    }
}

