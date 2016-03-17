#ifndef MODULE_H
#define MODULE_H

typedef struct {
    uint16_t clock;
    void (*setup)(void);
    void (*teardown)(void);
    void (*isr)(void);
} ledmodule_t;

extern ledmodule_t *ledmodules[];

void ledmodule_activate(uint8_t index);
void ledmodule_deactivate(uint8_t index);

#endif
