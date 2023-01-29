#ifndef POWER_H
#define POWER_H

extern unsigned char power_button_trig;

void power_init(void);
void start_main_clock(void);
void stop_main_clock(void);

typedef struct power {
    void (*init)(void);
    void (*start_main_clock)(void);
    void (*stop_main_clock)(void);
    void (*turn_on)(void);
    void (*turn_off)(void);
    unsigned char (*check_switch_state)(void);
} Power;
extern Power power;

#endif
