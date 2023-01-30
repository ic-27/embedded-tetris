#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include "power.h"
#include "tetris.h"

unsigned char power_button_trig;

/**
 * power_init() - Init pin for power switch
 *
 * Return: void
 */
void power_init(void)
{
    DDRD |= (1 << 7); // debug
    
    DDRB &= ~(1 << 2); // Set INT2 to be input pull-up
    PORTB |= (1 << 2);
}

/**
 * start_sys_clock() - Start the system clock, which controls the pace of the game
 *
 * Return: void
 */
void start_main_clock(void)
{
    TCCR1B |= (1 << CS12)|(1 << CS10); // start TIMER1, ISR(TIMER1_COMPA_vect)  
}

/**
 * stop_sys_clock() - Stop the system clock, which controls the pace of the game
 *
 * Return: void
 */
void stop_main_clock(void)
{
    TCCR1B &= ~((1 << CS12)|(1 << CS10));
}

/**
 * _enable_int() - Enable interrupt for power pin
 *
 * Return: void
 */
void _enable_int()
{
    GICR |= (1 << INT2);
}

/**
 * _disable_int() - Enable interrupt for power pin
 *
 * Return: void
 */
void _disable_int()
{
    GICR &= ~(1 << INT2);
}

/**
 * turn_on() - Power on the Atmega
 *
 * Return: void
 */
void turn_on(void)
{
    // config slide switch to look for rising edge
    MCUCSR |= (1 << ISC2); 
    reinit();
    _enable_int();
    power_button_trig = 0;
}

/**
 * power_down() - Put the Atmega into power down state
 *
 * Return: void
 */
void turn_off(void)
{
     // config slide switch to look for falling edge
    MCUCSR &= ~(1 << ISC2);
    deinit();
    stop_main_clock();
    power_button_trig = 0;
    _enable_int();
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    sleep_mode();
}

/**
 * check_switch_state() - Check which state slide switch is flipped to
 *
 * Return: boolean of whether on/off
 */
unsigned char check_switch_state(void)
{
    return !(PINB & (1 << 2));
}

/**
 * switch_trigger_action()
 *
 * Check the status of the switch and turn everything on/off upon a switch trigger
 *
 * Return: void
 */
void switch_trigger_action(void)
{
    if(power_button_trig) {
	_disable_int();
	if(check_switch_state()) {
	    turn_on();
	    return;
	}
	turn_off();
    }
}

/**
 * run_initial_switch_check()
 *
 * Run the initial check at the start when microcontroller is turned on for switch state
 *
 * Return: void
 */
void run_initial_switch_check(void)
{
    if(check_switch_state()) { // on
	turn_on();
	start_main_clock();
	return;
    }
    turn_off();
}

/**
 * External interrupt for detecting slide switch (power button)
 */
ISR(INT2_vect)
{
    if(!power_button_trig) {
	power_button_trig = 1;
	start_main_clock();
    }
}

Power power = {
    .init = &power_init,
    .run_initial_switch_check = &run_initial_switch_check
};
