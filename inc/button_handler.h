#ifndef BUTTON_HANDLER_H
#define BUTTON_HANDLER_H

#define BUT_INTERRUPT_PIN PB2
#define NUM_BUTTONS 5
#define DELAY 6

#define ROTATE_LEFT  PA1
#define ROTATE_RIGHT PA2
#define MOVE_LEFT    PA3
#define MOVE_DOWN    PA4
#define MOVE_RIGHT   PA5

#define ROTATE_LEFT_DEL  (PA1-1)
#define ROTATE_RIGHT_DEL (PA2-1)
#define MOVE_LEFT_DEL    (PA3-1)
#define MOVE_DOWN_DEL    (PA4-1)
#define MOVE_RIGHT_DEL   (PA5-1)

void init_buttons(void);
void config_common(void);
void config_distinct(void);

typedef struct button {
    void (*init)();
} Button;
extern Button button;

#endif
