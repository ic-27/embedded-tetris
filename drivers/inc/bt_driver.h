#ifndef BT_DRIVER_H
#define BT_DRIVER_H

#include "tetris.h"

#define BAUD_RATE	51	// 9600, see datasheet for table
#define BT_PIN		PD4
#define BT_SWITCH_PIN	PD6

typedef struct bluetooth {
    void (*init)(void);
    void (*tx)(char ch);
    unsigned char (*rx)(void);
    void (*on)(void);
    void (*off)(void);
    void (*toggle_on_switch)(void);
} Bluetooth;
extern Bluetooth bluetooth;

void bluetooth_on(void);
void bluetooth_off(void);

#endif
