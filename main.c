#include <avr/io.h>
#include <avr/eeprom.h>
#include "display_driver.h"
#include "spi_driver.h"

#define F_CPU 16000000UL
#include <util/delay.h>

/**
 * Macros for to set OCRn to to produce corresponding note/frequency, note this
 * is at 16 MHz clock frequency and 256 prescaler, formula on P74 of atmega32
 * datasheet.
 */
#define NOTE_GS3 149 // 208 Hz
#define NOTE_A3  141 // 220 Hz
#define NOTE_B3  126 // 247 Hz
#define NOTE_C4  118 // 262 Hz
#define NOTE_D4  105 // 294 Hz
#define NOTE_E4  94  // 330 Hz
#define NOTE_F4  89  // 349 Hz
#define NOTE_G4  79  // 392 Hz
#define NOTE_GS4 74  // 415 Hz
#define NOTE_A4  70  // 440 Hz


/**
 * init_audio() - initialize audio settings
 *
 * Set up the TIMER0 in CTC mode to continuously generate a square wave at a
 * frequency determined by note in the song.
 */
void init_audio(void)
{
    DDRB |= (1 << DDB3);
    TCCR0 = (1 << WGM01)|(1<<COM00)|(1 << CS02);
    OCR0 = NOTE_E4;
    TCNT0 = 0;
}

void change_note(unsigned char note)
{
    OCR0 = note;
    TCNT0 = 0;
}

int main(void)
{
    display.init();
    /* display.spi_send_cmd(OP_DIGIT2, 0xFF); */
    /* display.spi_send_cmd(OP_DIGIT3, 0xFF); */

    // testing CTC
    init_audio();
    _delay_ms(500);
    change_note(NOTE_B3);
    _delay_ms(250);
    change_note(NOTE_C4);
    _delay_ms(250);
    change_note(NOTE_D4);
    _delay_ms(500);
    change_note(NOTE_C4);
    _delay_ms(250);
    change_note(NOTE_B3);
    _delay_ms(250);
    change_note(NOTE_A3);
    _delay_ms(500);
    change_note(NOTE_A3);
    _delay_ms(250);
    change_note(NOTE_C4);
    _delay_ms(250);
    change_note(NOTE_E4);
    _delay_ms(500);        

    for(;;) {
	
    }
}

//eeprom_update_byte(0, 0xEE);
