#include <avr/io.h>
#include <avr/eeprom.h>
#include <avr/interrupt.h>
#include "button_handler.h"
#include "display_driver.h"
#include "spi_driver.h"
#include "audio_driver.h"

#include "common.h"
#include <util/delay.h>

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
