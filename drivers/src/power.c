#include <avr/io.h>
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
    //MCUCSR |= (1 << ISC2); 

    //reinit();
    
    _enable_int();
    //power_button_trig = 0;
}

/**
 * power_down() - Put the Atmega into power down state
 *
 * Return: void
 */
void turn_off(void)
{
    //power_button_trig = 0;
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
 * check_on_switch_trigger()
 *
 * Check the status of the switch and turn everything on/off upon a switch trigger
 *
 * Return: void
 */
void check_on_switch_trigger(void)
{
    if(power_button_trig) {
	_disable_int();
	if(check_switch_state()) {
	    // call power_on function
	    return;
	}
	// call power_off function
    }
}

/* void check_switch_status(void) */
/* { */
/*     if(power_button_trig) { */
/* 	power_disable_int(); */
/* 	if(check_switch_state()) { */
/* 	    power_on(); */
/* 	} else { */
/* 	    power_off(); */
/* 	} */
/*     }     */
/* } */

/**
 * External interrupt for detecting slide switch (power button)
 */
/* ISR(INT2_vect) */
/* { */
/*     if(!power_button_trig) { */
/* 	power_button_trig = 1; */
/* 	start_main_clock(); */
/*     } */
/* } */

Power power = {
    .init = &power_init,
    .start_main_clock = &start_main_clock,
    .stop_main_clock = &stop_main_clock,
    .turn_on = &turn_on,
    .turn_off = &turn_off,
    .check_switch_state = &check_switch_state
};
