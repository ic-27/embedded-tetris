#include <avr/io.h>
#include <avr/eeprom.h>
#include <avr/interrupt.h>
#include "button_handler.h"
#include "display_driver.h"
#include "spi_driver.h"
#include "audio_driver.h"

#include "common.h"
#include <util/delay.h>

#warning how do you find out the name of the interrupt vector?
ISR(INT2_vect)
{
    config_distinct();

    unsigned char temp_PINA = PINA; // save before PINA changes
    if(temp_PINA  & (1 << BUTTON_LEFT)) {
	PORTA |= (1 << 0);
    } else if(temp_PINA & (1 << BUTTON_RIGHT)) {
	PORTA &= ~(1 << 0);
    }
    _delay_ms(100);


    config_common();
    GIFR |= (1 << INTF2);
}

ISR(TIMER1_COMPA_vect)
{
    // Audio functionality, switch to a different note based on tetris_melody
    if(tetris_melody[note]) {
	audio.change_note(tetris_melody[note]);
    }

    ++note;
    if(note >= tetris_melody_length) {
	note = 0;
    }
    // Drop a piece every 500 ms
}

int main(void)
{
    sei();
    /* display.init(); */
    /* audio.init(); */
    /* audio.play(); */
    init_buttons();

    DDRA |= (1 << 0);

    /* _delay_ms(500); */
    /* audio.change_note(NOTE_A4); */
    /* _delay_ms(500); */
    /* audio.stop(); */

    /* display.spi_send_cmd(OP_DIGIT2, 0xFF); */
    /* display.spi_send_cmd(OP_DIGIT3, 0xFF); */
    for(;;) {

	}
}

//eeprom_update_byte(0, 0xEE);
