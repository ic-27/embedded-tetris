#ifndef AUDIO_DRIVER_H
#define AUDIO_DRIVER_H

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

/* #define PLAY() (TCCR0 |=  (1 << CS02)) */
/* #define STOP() (TCCR0 &= ~(1 << CS02)) */

void init_audio(unsigned char starting_note);
void play_audio(void);
void stop_audio(void);
void change_note(unsigned char note);

typedef struct audio {
    void (*init)(unsigned char starting_note);
    void (*play)(void);
    void (*stop)(void);
    void (*change_note)(unsigned char note);
} Audio;
extern Audio audio;

#endif
