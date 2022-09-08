#ifndef BUTTON_HANDLER_H
#define BUTTON_HANDLER_H

#define BUT_INTERRUPT_PIN PB2
#define NUM_BUTTONS 5

#define ROTATE_LEFT  PA1
#define ROTATE_RIGHT PA2
#define MOVE_LEFT    PA3
#define MOVE_DOWN    PA4
#define MOVE_RIGHT   PA5

void init_buttons(void);
void config_common(void);
void config_distinct(void);

#endif
