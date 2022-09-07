#include <avr/io.h>
#include "button_handler.h"

static unsigned char button_pressed = 0;
static unsigned int debounce_state[NUM_BUTTONS] = {0}; // record last time when pressed

void init_buttons(void)
{
    DDRB &= ~(1 << DDB2); // DDB2 is button interrupt pin
    PORTB |= (1 << PB2); // Configure pin w/ pull-up resistor

    DDRA |= 0b00111110; // Configure the 5 buttons as outputs
    PORTA &= ~(0b00111110);

    GICR |= (1 << INT2);
}

/**
 * config_common()
 * 
 * Configure the interrupt pin as an input w/ a pull-up, and the 5 buttons as
 * outputs which default to 0. See configure_distinct() function to understand
 * how the button mechanism works.
 *
 * Return: void
 */
void config_common(void)
{
    PORTB |= (1 << PB2);
    DDRB  &= ~(1 << DDB2); // DDB2 is button interrupt pin
    PORTB |= (1 << PB2); // Configure pin w/ pull-up resistor

    DDRA |= 0b00111110; // Configure the 5 buttons as outputs
    PORTA &= ~(0b00111110);

    //GICR |= (1 << INT2);
}

/**
 * configure_distinct()
 *
 * Configure the interrupt pin to logic low output, and the 5 buttons as inputs
 * w/ pull-ups. This function is called immediately after an interrupt. Using
 * this, we can determine which buttons have been pressed in the interrupt.
 *
 * Return: void
 */
void config_distinct(void)
{
    //GICR &= ~(1 << INT2);

    PORTB &= ~(1 << PB2);
    DDRB |= (1 << DDB2); // Configure pin as output
    PORTB &= ~(1 << PB2); // Configure pin as low output

    DDRA &= ~(0b00111110); // Configure the 5 buttons as inputs
    PORTA |= 0b00111110; // Configure pins w/ pull-up resistor
}


/* Need to understand how to interface multiple buttons to a single interrupt (or 2) 

*/


/* Need an indication of when the debounce time has passed for a certain button 

   First, we need to realize that we have multiple buttons mapped to a single
   interrupt.  Only one interrupt is edge-triggered, so we probably want to only
   use that one.  Level-triggered interrupts are problematic because they will
   keep calling the interrupt if they are at that logic level, and we don't want
   to keep interrupting the cpu from performing other tasks.

   This means we will have 5 buttons mapped to a single interrupt.  Say that we
   are able to detect which button has been pressed in the interrupt.  We need a
   way to debounce that certain button for a certain amount of time.

   Once a button has been detected (interrupt has been triggered), start a
   timer. The timer will only run if there has been a button pressed, and will
   stop when it detects that no buttons have been pressed. This is such that we
   do not waste any precious cpu cycles when handling the timer interrupt. This
   is also nice because there will be no overflow,

   The purpose of the timer interrupt is to perform button debouncing, and allow
   the user to invoke an action again after a certain amount of time has passed.
   The rate at which the timer will run is 25ms to increase the resolution of
   the timing of when something will be debounced. The debounce period will
   probably be somewhere between 100ms-250ms, there will be some trial and
   error.

   We can use a counter, for how many 25ms have passed and log it into the array
   debounce_state. If the value in the array is not 0, then that means it is
   currently debouncing. We can put the timer into CTC mode and keep triggering
   an interrupt every 25ms, which shouldn't be very noticeable if the user
   happens to press a button when the timer has already started!
*/

