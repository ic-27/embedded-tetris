#ifndef BUTTON_HANDLER_H
#define BUTTON_HANDLER_H

#define BUT_INTERRUPT_PIN PB2
#define NUM_BUTTONS 5

#define BUTTON_LEFT  PB1
#define BUTTON_RIGHT PB2
#define BUTTON_DOWN  PB3

void init_buttons(void);
void config_common(void);
void config_distinct(void);

#endif
