#include <avr/io.h>
#include <avr/eeprom.h>
#include <avr/interrupt.h>

#include "tetris.h"

int main(void)
{
    sei();
    init_tetris();

    for(;;) {
	if(NEXT_MOVE_READY == next_move) {
	    next_move_logic();
	}
    }
}
/* uart_tx('1'); */
//eeprom_update_byte(0, 0xEE);
