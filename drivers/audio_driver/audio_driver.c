#include <avr/io.h>
#include <avr/interrupt.h>
#include "audio_driver.h"

unsigned char note = 0;
unsigned char tetris_melody[] = {
    NOTE_E4, 0, NOTE_B3, NOTE_C4, NOTE_D4, // first part
    0, NOTE_C4, NOTE_B3, NOTE_A3, 0,
    NOTE_A3, NOTE_C4, NOTE_E4, 0, NOTE_D4,
    NOTE_C4, NOTE_B3, 0, NOTE_B3, NOTE_C4,
    NOTE_D4, 0, NOTE_E4, 0, NOTE_C4,
    0, NOTE_A3, 0, NOTE_A3, 0,
    0, 0, NOTE_D4, 0, 0,
    NOTE_F4, NOTE_A4, 0, NOTE_G4, NOTE_F4,
    NOTE_E4, 0, 0, NOTE_C4, NOTE_E4,
    0, NOTE_D4, NOTE_C4, NOTE_B3, 0,
    NOTE_B3, NOTE_C4, NOTE_D4, 0, NOTE_E4,
    0, NOTE_C4, 0, NOTE_A3, 0,
    NOTE_A3, 0, 0, 0,

    NOTE_E4, 0, 0, 0, NOTE_C4, // second part
    0, 0, 0, NOTE_D4, 0,
    0, 0, NOTE_B3, 0, 0,
    0, NOTE_C4, 0, 0, 0,
    NOTE_A3, 0, 0, 0, NOTE_GS3,
    0, 0, 0, NOTE_B3, 0,
    0, 0, NOTE_E4, 0, 0,
    0, NOTE_C4, 0, 0, 0,
    NOTE_D4, 0, 0, 0, NOTE_B3,
    0, 0, 0, NOTE_C4, 0,
    NOTE_E4, 0, NOTE_A4, 0, NOTE_A4,
    0, NOTE_GS4, 0, 0, 0
};
const unsigned char tetris_melody_length = sizeof(tetris_melody)/sizeof(char);

/**
 * init_audio() - initialize audio settings
 *
 * Set up TIMER0 and TIMER1, which will be used to generate audio.
 * Note that TIMER1 may also be used to control the pace of the tetris game,
 * this will be noted in the ISR of TIMER1.
 *
 * Return: void
 */
void init_audio()
{
    // Initialize 8-bit TIMER0, which will gen a sqr wave to produce note.
    // play_audio() will handle setting init freq and starting the waveform.
    DDRB |= (1 << DDB3); //Output on PB3
    TCCR0 = (1 << WGM01)|(1<<COM00); // audio only starts when CS02 is set
    TCNT0 = 0;

    // Initialize 16-bit TIMER1, which will be a 250 ms timer to control the
    // length of each note, amongst other things. See the ISR definition.
    TCCR1A = 0;
    TCCR1B = (1 << WGM12);//|(1 << CS12)|(1 << CS10);
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
 * Reset the melody to start the from the beginning and play.
 * It is very important to note that this function is also what starts TIMER1,
 * in which the ISR of TIMER1 does other things than just play the melody.
 * See TIMER1_COMPA_vect for what else it does.
 * The logic behind this is that audio should only be played when user is
 * playing the game, and not in sleep mode or something, so don't need to
 * necessarily separate audio w/ whatever other functionality ISR provides.
 *
 * Return: void
 */
void play_audio()
{
    note = 0; // reset melody
    OCR0 = tetris_melody[note++];

    TCCR0  |= (1 << CS02); // start TIMER0 (waveform generation for buzzer)
    TCCR1B |= (1 << CS12)|(1 << CS10); // start TIMER1, ISR(TIMER1_COMPA_vect)
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
