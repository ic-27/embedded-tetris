#include <avr/io.h>
#include <avr/interrupt.h>
#include "bt_driver.h"

void init_bluetooth(void)
{
    UBRRH = (unsigned char)(BAUD_RATE>>8);
    UBRRL = (unsigned char)BAUD_RATE;

    UCSRB = (1<<RXEN)|(1<<TXEN) |(1<<RXCIE); // enable tx, rx, int
    UCSRC = (1<<URSEL) | (1<<UCSZ0)|(1<<UCSZ1); // async, 8-bit

    DDRC |= (1 << PC0);
    PORTC |= (1 << PC0);
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
    PORTD |= (1 << PD6);
}

void bluetooth_off(void)
{
    PORTD &= ~(1 << PD6);
}

Bluetooth bluetooth = {
    .init = &init_bluetooth,
    .tx = &uart_tx,
    .rx = &uart_rx
};

ISR(USART_RXC_vect)
{
    char c = bluetooth.rx();
    bluetooth.tx(c);
}
