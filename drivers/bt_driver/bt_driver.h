#ifndef BT_DRIVER_H
#define BT_DRIVER_H

#define BAUD_RATE 51 // 9600, see datasheet for table

typedef struct uart {
    void (*init)(void);
    void (*tx)(char ch);
    unsigned char (*rx)(void);
} UART;
extern UART uart;

void init_uart(void);
void uart_tx(char ch);
unsigned char uart_rx(void);

#endif
