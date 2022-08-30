#include <avr/io.h>
#include <avr/interrupt.h>
#include "audio_driver.h"

/**
 * init_audio() - initialize audio settings
 *
 * Set up TIMER0 and TIMER1, which will be used to generate audio.
 * Note that TIMER1 may also be used to control the pace of the tetris game,
 * this will be noted in the ISR of TIMER1.
 *
 * Return: void
 */
void init_audio(unsigned char starting_note)
{
    // Initialize 8-bit TIMER0, which will generate the sqr wave to produce note
    DDRB |= (1 << DDB3); //Output on PB3
    TCCR0 = (1 << WGM01)|(1<<COM00);
    OCR0  = starting_note;
    TCNT0 = 0;

    // Initialize 16-bit TIMER1, which will be a 250 ms timer to control the
    // length of each note amongst other things. See the ISR definition.
    TCCR1A = 0;
    TCCR1B = (1 << WGM12)|(1 << CS12)|(1 << CS10);
    OCR1A = 3905; // 250 ms
    TCNT1 = 0;
    // Enable the interrupt for TIMER1
#warning probably better placement for these rather than in init_audio()
    sei();
    TIMSK = (1 << OCIE1A); // enable interrupt
    TIFR  = (1 << OCF1A);  // allow execution of interrupt
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
