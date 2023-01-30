#include <avr/io.h>
#include <avr/eeprom.h>
#include <avr/interrupt.h>
#include "tetris.h"
#include "power.h"

int main(void)
{
    sei();
    GICR &= ~((1 << INT0) | (1 << INT1) | (1 << INT2)); // disable interrupts initially
    start_tetris();

    for(;;) {
	if(NEXT_MOVE_READY == next_move) {
	    switch_trigger_action(); // function put here to allow debouncing, since functions in this if statement are run every 500ms
	    next_move_logic();
	}
    }
}

//eeprom_update_byte(0, 0xEE);
