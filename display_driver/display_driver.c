/**
 * FILENAME: display_driver.c
 *
 * DESCRIPTION:
 * This is the display driver for the 2 8x8 dot matrix displays. Two MAX7219 are
 * used mainly to help display the contents at 80 MHz.
 */

#include "display_driver.h"
#include "spi_driver.h"

/**
 * _spi_init() - Initialize SPI settings
 *
 * TODO: Find out the initial state of the registers upon boot-up!
 *
 * Return: void
 */
static void _spi_init(void)
{
    // MSB, f_osc/2
    DDRB = (1<<MOSI)|(1<<SCK)|(1<<SS);
    SPCR = (1<<SPE)|(1<<MSTR);
    SPSR = (1<<SPI2X);
}

/**
 * init() - Initialize display communication and settings
 *
 * Configure spi settings, and the initial settings of the MAX7219.
 *
 * Return: void
 */
void init_display(void)
{
    _spi_init();
    spi_send_cmd_num(OP_SHUTDOWN, 1);
    spi_send_cmd_num(OP_DISPLAY_TEST, 0);
    spi_send_cmd_num(OP_DECODE_MODE, 0);
    spi_send_cmd_num(OP_INTENSITY, 2);
    spi_send_cmd_num(OP_SCAN_LIMIT, 7);
    clear();
}

/**
 * spi_send_cmd() - Send a command to a single MAX7219
 * @cmd:  byte containing command
 * @data: byte containing data to send
 *
 * Send 2 byte command to a single MAX7219, with each byte sent separately.
 *
 * Return: void
 */
void spi_send_cmd(unsigned char cmd, unsigned char data)
{
    PORTB &= ~(1<<SS);

    SPDR = cmd;
    while(!(SPSR & (1<<SPIF))); // wait for transmission to be done
    SPDR = data;
    while(!(SPSR & (1<<SPIF))); // wait for transmission to be done

    /* SPDR = OP_NOOP; */
    /* while(!(SPSR & (1<<SPIF))); */
    /* SPDR = OP_NOOP; */
    /* while(!(SPSR & (1<<SPIF))); */

    PORTB |= (1<<SS);
}

/**
 * spi_send_cmd_top()
 * @cmd:  byte containing command
 * @data: byte containing data to send
 *
 * Send 2 byte command to the top led matrix
 *
 * Return: void
 */
void spi_send_cmd_top(unsigned char cmd, unsigned char data)
{
    PORTB &= ~(1<<SS);

    SPDR = OP_NOOP;
    while(!(SPSR & (1<<SPIF)));
    SPDR = OP_NOOP;
    while(!(SPSR & (1<<SPIF)));

    SPDR = cmd;
    while(!(SPSR & (1<<SPIF))); // wait for transmission to be done
    SPDR = data;
    while(!(SPSR & (1<<SPIF))); // wait for transmission to be done

    PORTB |= (1<<SS);
}

/**
 * spi_send_cmd_bot()
 * @cmd:  byte containing command
 * @data: byte containing data to send
 *
 * Send 2 byte command to the bottom led matrix
 *
 * Return: void
 */
void spi_send_cmd_bot(unsigned char cmd, unsigned char data)
{
    PORTB &= ~(1<<SS);

    SPDR = cmd;
    while(!(SPSR & (1<<SPIF))); // wait for transmission to be done
    SPDR = data;
    while(!(SPSR & (1<<SPIF))); // wait for transmission to be done

    SPDR = OP_NOOP;
    while(!(SPSR & (1<<SPIF)));
    SPDR = OP_NOOP;
    while(!(SPSR & (1<<SPIF)));

    PORTB |= (1<<SS);
}

/**
 * spi_send_cmd_num() - Send a command to multiple MAX7219
 * @cmd:  byte containing command
 * @data: byte containing data to send
 *
 * Send 2 byte command to a single MAX7219, with each byte sent separately.
 * Command is sent to a number of MAX7219 defined as NUM_DISPLAYS.
 *
 * Note that if you have multiple displays and only want to send a command to
 * the last display, you will need to send the command, then send NO-OP commands
 * for the remaining displays, and then latch.
 *
 * Return: void
 */
void spi_send_cmd_num(unsigned char cmd, unsigned char data)
{
    PORTB &= ~(1<<SS);

    for(unsigned char i = 0; i<NUM_DISPLAYS; ++i) {
	SPDR = cmd;
	while(!(SPSR & (1<<SPIF))); // wait for transmission to be done
	SPDR = data;
	while(!(SPSR & (1<<SPIF))); // wait for transmission to be done
    }

    PORTB |= (1<<SS);
}

/**
 * refresh() - Refresh the display
 *
 * Refresh the display to show whatever changes have been made.
 *
 * Return: void
 */
void refresh(void)
{
    
}

/**
 * clear() - Clear 8x8 dot matrices
 *
 * Send commands to entirely clear the 2 8x8 dot matrices.
 *
 * Return: void
 */
void clear(void)
{
    spi_send_cmd_num(ROW0, 0x00);
    spi_send_cmd_num(ROW1, 0x00);
    spi_send_cmd_num(ROW2, 0x00);
    spi_send_cmd_num(ROW3, 0x00);
    spi_send_cmd_num(ROW4, 0x00);
    spi_send_cmd_num(ROW5, 0x00);
    spi_send_cmd_num(ROW6, 0x00);
    spi_send_cmd_num(ROW7, 0x00);
}

Display display = {
    .init = &init_display,
    .spi_send_cmd = &spi_send_cmd,
    .spi_send_cmd_top = &spi_send_cmd_top,
    .spi_send_cmd_bot = &spi_send_cmd_bot,
    .spi_send_cmd_num = &spi_send_cmd_num,
    .clear = &clear
};
