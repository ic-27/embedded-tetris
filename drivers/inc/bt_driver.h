#ifndef BT_DRIVER_H
#define BT_DRIVER_H

#define BAUD_RATE 51 // 9600, see datasheet for table

typedef struct bluetooth {
    void (*init)(void);
    void (*tx)(char ch);
    unsigned char (*rx)(void);
    void (*on)(void);
    void (*off)(void);
} Bluetooth;
extern Bluetooth bluetooth;

void init_uart(void);
void uart_tx(char ch);
unsigned char uart_rx(void);
void bluetooth_on(void);
void bluetooth_off(void);

#endif
