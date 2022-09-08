#include "common.h"
#include <util/delay.h>

#include <avr/io.h>
#include <avr/interrupt.h>
#include "button_handler.h"

static unsigned char delayed = 0;
static unsigned char delay_state[NUM_BUTTONS] = {0};

#warning try to find a more efficient way for this

/**
 * ISR TIMER2
 *
 * This ISR is used for the polling of the buttons.

 * Check for a button press every 30 ms. If a button is pressed, give
 * it a delay of (DELAY-2) * 30 ms before it can be pressed again.
 */
ISR(TIMER2_COMP_vect)
{
    if(!(PINA & (1 << MOVE_LEFT)) && !delay_state[MOVE_LEFT_DEL])
	{
	    PORTD ^= (1 << PD0);
	    delay_state[MOVE_LEFT_DEL] = 1;
	}
    else if(!(PINA & (1 << MOVE_RIGHT)) && !delay_state[MOVE_RIGHT_DEL])
	{
	    PORTD ^= (1 << PD0);
	    delay_state[MOVE_RIGHT_DEL] = 1;
	}
    else if(!(PINA & (1 << ROTATE_LEFT)) && !delay_state[ROTATE_LEFT_DEL])
	{
	    PORTD ^= (1 << PD0);
	    delay_state[ROTATE_LEFT_DEL] = 1;
	}
    else if(!(PINA & (1 << ROTATE_RIGHT)) && !delay_state[ROTATE_RIGHT_DEL])
	{
	    PORTD ^= (1 << PD0);
	    delay_state[ROTATE_RIGHT_DEL] = 1;
	}
    else if(!(PINA & (1 << MOVE_DOWN)) && !delay_state[MOVE_DOWN_DEL])
	{
	    PORTD ^= (1 << PD0);
	    delay_state[MOVE_DOWN_DEL] = 1;
	}

    // check if delay has ended
    if(delay_state[MOVE_LEFT_DEL] && delay_state[MOVE_LEFT_DEL] < DELAY) {
	++delay_state[MOVE_LEFT_DEL];
    } else {
	delay_state[MOVE_LEFT_DEL] = 0;
    }

    if(delay_state[MOVE_RIGHT_DEL] && delay_state[MOVE_RIGHT_DEL] < DELAY) {
	++delay_state[MOVE_RIGHT_DEL];
    } else {
	delay_state[MOVE_RIGHT_DEL] = 0;
    }

    if(delay_state[ROTATE_LEFT_DEL] && delay_state[ROTATE_LEFT_DEL] < DELAY) {
	++delay_state[ROTATE_LEFT_DEL];
    } else {
	delay_state[ROTATE_LEFT_DEL] = 0;
    }

    if(delay_state[ROTATE_RIGHT_DEL] && delay_state[ROTATE_RIGHT_DEL] < DELAY) {
	++delay_state[ROTATE_RIGHT_DEL];
    } else {
	delay_state[ROTATE_RIGHT_DEL] = 0;
    }

    if(delay_state[MOVE_DOWN_DEL] && delay_state[MOVE_DOWN_DEL] < DELAY) {
	++delay_state[MOVE_DOWN_DEL];
    } else {
	delay_state[MOVE_DOWN_DEL] = 0;
    }
}

/**
 * init_buttons()
 *
 * Initialize buttons as inputs w/ internal pull-ups.
 * PA0 is chosen for ADC, which is why it is not used.
 *
 * A polling method will be used instead of an interrupt method. Given
 * the rate at which the user will be pressing the buttons and that
 * there is only a small benefit in using interrupts, a polling method
 * will suffice.
 *
 * Return: void
 */
void init_buttons(void)
{
    DDRA &= ~(0b00111110);
    PORTA |= 0b00111110;

    // Initialize timer0 for polling method.
    TCCR2 = (1 << WGM21) | (1 << CS22) | (1 << CS21) | (1 << CS20);
    OCR2 = 234; // 30 ms
    TCNT2 = 0;

    TIMSK |= (1 << OCIE2);
    TIFR |= (1 << OCF2);
}
