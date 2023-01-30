#ifndef POWER_H
#define POWER_H

extern unsigned char power_button_trig;

void power_init(void);
void start_main_clock(void);
void stop_main_clock(void);
void switch_trigger_action(void);

typedef struct power {
    void (*init)(void);
    void (*run_initial_switch_check)(void);
} Power;
extern Power power;

#endif
