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

/**
 * ISR2
 *
 * ISR2 handles all the button input. ISR2 is picked because it can wake up the
 * microcontroller from power-down mode w/o presence of io_clk unlike INT0 and
 * INT1.
 *
 * All of the button inputs are handled here instead of breaking it up into
 * another interrupt because the microcontroller can handle it fast enough.
 */
ISR(INT2_vect)
{
    config_distinct();

    unsigned char temp_PINA = PINA; // save before PINA changes
    if(temp_PINA & (1 << ROTATE_LEFT)) {
	PORTD ^= _BV(PD0);
    }
    if(temp_PINA & (1 << ROTATE_RIGHT)) {
	PORTD ^= _BV(PD1);
    }
    if(temp_PINA & (1 << MOVE_LEFT)) {// me and buttons below me always go into rotate_left
	PORTD ^= _BV(PD2);
    }
    if(temp_PINA & (1 << MOVE_DOWN)) {
	PORTD ^= _BV(PD3);
    }
    if(temp_PINA & (1 << MOVE_RIGHT)) {
	PORTD ^= _BV(PD4);
    }
    _delay_ms(250);

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
    display.init();
    audio.init();
    //audio.play();
    init_buttons();

    DDRD |= 0xFF;

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
