#include <avr/io.h>
#include <avr/eeprom.h>
#include <avr/interrupt.h>

#include "bt_driver.h"
#include "button_handler.h"
#include "display_driver.h"
#include "audio_driver.h"

#include "tetris.h"
#include "display.h"

#include "common.h"
#include <util/delay.h>

int main(void)
{
    sei();
    init_tetris();
    /* display.init(); */
    /* audio.init(); */
    /* audio.play(); */
    /* button.init(); */

    /* bluetooth.init(); */

    /* _delay_ms(500); */
    /* audio.change_note(NOTE_A4); */
    /* _delay_ms(500); */
    /* audio.stop(); */

    /* display.spi_send_cmd_top(OP_DIGIT2, 0x77); */
    //display.spi_send_cmd_bot(OP_DIGIT7, 0x7F);
    DDRD |= (1 << PD2);

    for(;;) {
	next_state_logic();
	/* uart_tx('1'); */
	/* _delay_ms(250); */

    }
}

//eeprom_update_byte(0, 0xEE);
