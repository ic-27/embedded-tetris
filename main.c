#include <avr/io.h>
#include <avr/eeprom.h>
#include "display_driver.h"
#include "spi_driver.h"
#include "audio_driver.h"

#define F_CPU 16000000UL
#include <util/delay.h>

int main(void)
{
    display.init();
    audio.init(NOTE_E4);
    audio.play();
    _delay_ms(500);
    audio.change_note(NOTE_A4);
    _delay_ms(500);
    audio.stop();
    /* display.spi_send_cmd(OP_DIGIT2, 0xFF); */
    /* display.spi_send_cmd(OP_DIGIT3, 0xFF); */

    for(;;) {
    }
}

//eeprom_update_byte(0, 0xEE);
