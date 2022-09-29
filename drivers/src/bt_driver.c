#include <avr/io.h>
#include <avr/interrupt.h>
#include "bt_driver.h"

void init_bluetooth(void)
{
    UBRRH = (unsigned char)(BAUD_RATE>>8);
    UBRRL = (unsigned char)BAUD_RATE;

    UCSRB = (1<<RXEN)|(1<<TXEN) |(1<<RXCIE); // enable tx, rx, int
    UCSRC = (1<<URSEL) | (1<<UCSZ0)|(1<<UCSZ1); // async, 8-bit

    DDRD  |= (1 << BT_PIN); // set up BT, controlled through npn transistor
    bluetooth_on();
}

void uart_tx(char ch)
{
    while (! (UCSRA & (1<<UDRE)));  /* Wait for empty transmit buffer */
    UDR = ch;
}

unsigned char uart_rx(void)
{
    while ((UCSRA & (1 << RXC)) == 0);/* Wait till data is received */
    return UDR;
}

void bluetooth_on(void)
{
    PORTD |= (1 << BT_PIN);
}

void bluetooth_off(void)
{
    PORTD &= ~(1 << BT_PIN);
}

Bluetooth bluetooth = {
    .init = &init_bluetooth,
    .tx   = &uart_tx,
    .rx   = &uart_rx,
    .on   = &bluetooth_on,
    .off  = &bluetooth_off
};

ISR(USART_RXC_vect)
{
    char c = bluetooth.rx();

    switch(c) {
    case '4':
	damage += 4;
	break;
    case '2':
	damage += 2;
	break;
    case '1':
	damage += 1;
	break;
    default:
	break;
    }
}
