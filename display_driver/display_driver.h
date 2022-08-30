#ifndef DISPLAY_DRIVER_H
#define DISPLAY_DRIVER_H 1

/* MAX7219 Commands */
#define OP_NOOP          0
#define OP_DIGIT0        1
#define OP_DIGIT1        2
#define OP_DIGIT2        3
#define OP_DIGIT3        4
#define OP_DIGIT4        5
#define OP_DIGIT5        6
#define OP_DIGIT6        7
#define OP_DIGIT7        8
#define OP_DECODE_MODE   9
#define OP_INTENSITY    10
#define OP_SCAN_LIMIT   11
#define OP_SHUTDOWN     12
#define OP_DISPLAY_TEST 15

/* Other Macros */
#define ROW0 OP_DIGIT0
#define ROW1 OP_DIGIT1
#define ROW2 OP_DIGIT2
#define ROW3 OP_DIGIT3
#define ROW4 OP_DIGIT4
#define ROW5 OP_DIGIT5
#define ROW6 OP_DIGIT6
#define ROW7 OP_DIGIT7

#define NUM_DISPLAYS 2

typedef struct display {
    void (*init)(void);
    void (*spi_send_cmd)(unsigned char cmd, unsigned char data);
    void (*spi_send_cmd_num)(unsigned char cmd, unsigned char data);
    void (*clear)(void);
} Display;
extern Display display;

void init(void);
void refresh(void);
void spi_send_cmd(unsigned char cmd, unsigned char data);
void spi_send_cmd_num(unsigned char cmd, unsigned char data);
void clear(void);

#endif
