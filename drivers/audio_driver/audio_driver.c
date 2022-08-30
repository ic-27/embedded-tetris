#include <avr/io.h>
#include "audio_driver.h"

/**
 * init_audio() - initialize audio settings
 *
 * Set up the TIMER0 in CTC mode to continuously generate a square wave at a
 * frequency determined by note in the song.
 *
 * Return: void
 */
void init_audio(unsigned char starting_note)
{
    DDRB |= (1 << DDB3); //Output on PB3
    TCCR0 = (1 << WGM01)|(1<<COM00); //|(1 << CS02);
    OCR0 = starting_note;
    TCNT0 = 0;
}

/**
 * play_audio() - start audio by starting timer/counter
 *
 * Return: void
 */
void play_audio(void)
{
    TCCR0 |=  (1 << CS02);
}

/**
 * stop_audio() - stop audio by stopping timer/counter
 *
 * Note that a better way is to also set CS00 and CS01 to 0, but we only ever
 * use CS02, so it does not really matter.
 *
 * Return: void
 */
void stop_audio(void)
{
    TCCR0 &= ~(1 << CS02);
}

/**
 * change_note()
 * @note: Note/frequency to change to, see audio_driver.h
 *
 * Set note to change to and reset timer/counter register.
 * Note that this function is for use only w/ TIMER0.
 *
 * Return: void
 */
void change_note(unsigned char note)
{
    OCR0 = note;
    TCNT0 = 0;
}

Audio audio = {
    .init = &init_audio,
    .play = &play_audio,
    .stop = &stop_audio,
    .change_note = &change_note
};
