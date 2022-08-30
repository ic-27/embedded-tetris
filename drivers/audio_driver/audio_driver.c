#include "audio_driver.h"

/**
 * init_audio() - initialize audio settings
 *
 * Set up the TIMER0 in CTC mode to continuously generate a square wave at a
 * frequency determined by note in the song.
 * 
 * Return: void
 */
void init_audio(void)
{
    DDRB |= (1 << DDB3); //Output on PB3
    TCCR0 = (1 << WGM01)|(1<<COM00)|(1 << CS02);
    OCR0 = NOTE_E4;
    TCNT0 = 0;
}

void play(void)
{
    
}

/**
 * change_note()
 * @note: Note/frequency to change to, see audio_driver.h
 *
 * Set note to change to and reset timer/counter register.
 * Note that this function is for use only w/ TIMER0.
 * Return: void
 */
void change_note(unsigned char note)
{
    OCR0 = note;
    TCNT0 = 0;
}
