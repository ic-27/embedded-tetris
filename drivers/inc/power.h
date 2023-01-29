#ifndef POWER_H
#define POWER_H

void power_init(void);
void start_main_clock(void);
void stop_main_clock(void);

typedef struct power {
    void (*init)(void);
    void (*start_main_clock)(void);
    void (*stop_main_clock)(void);
    void (*power_down)(void);
} Power;
extern Power power;

#endif
