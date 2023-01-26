#ifndef AUDIO_DRIVER_H
#define AUDIO_DRIVER_H

#include "common.h"

/* TIM0 MACROS */
#define TIM0_PRESCALER 256
/**
 * HZ_TO_OCRn() - Convert Hz to OCRn
 * @hz: hertz to generate
 *
 * Convert Hz to generate note at to OCRn, or the timer count which will
 * generate a square wave.
 *
 * Note that calculations are based on the prescaler, so make sure they match.
 *
 * Return: void
 */
#define HZ_TO_OCRn(hz) ((F_CPU/hz/(2*TIM0_PRESCALER)-1))

// Notes defined in terms of hertz
#define NOTE_GS3_HZ 208
#define NOTE_A3_HZ  220
#define NOTE_B3_HZ  247
#define NOTE_C4_HZ  262
#define NOTE_D4_HZ  294
#define NOTE_E4_HZ  330
#define NOTE_F4_HZ  349
#define NOTE_G4_HZ  392
#define NOTE_GS4_HZ 415
#define NOTE_A4_HZ  440

// Notes defined in terms of OCRn
#define NOTE_GS3 HZ_TO_OCRn(NOTE_GS3_HZ)
#define NOTE_A3  HZ_TO_OCRn(NOTE_A3_HZ)
#define NOTE_B3  HZ_TO_OCRn(NOTE_B3_HZ)
#define NOTE_C4  HZ_TO_OCRn(NOTE_C4_HZ)
#define NOTE_D4  HZ_TO_OCRn(NOTE_D4_HZ)
#define NOTE_E4  HZ_TO_OCRn(NOTE_E4_HZ)
#define NOTE_F4  HZ_TO_OCRn(NOTE_F4_HZ)
#define NOTE_G4  HZ_TO_OCRn(NOTE_G4_HZ)
#define NOTE_GS4 HZ_TO_OCRn(NOTE_GS4_HZ)
#define NOTE_A4  HZ_TO_OCRn(NOTE_A4_HZ)

/* TIM1 MACROS */
#define TIM1_PRESCALER 1024
#define CHANGE_PERIOD 0.250 // 250 ms to switch to a different note
#define OCR1A_VAL ((int)(((float)CHANGE_PERIOD/(1/((float)F_CPU/TIM1_PRESCALER)))-1))

extern unsigned char note; // keep track of where we are in tetris_melody
extern unsigned char tetris_melody[];
extern const unsigned char tetris_melody_length;

typedef struct audio {
    void (*init)();
    void (*play)(void);
    void (*stop)(void);
    void (*start_clock)(void);
    void (*stop_clock)(void);
    void (*change_note)(unsigned char note);
} Audio;
extern Audio audio;

void init_audio();
void play_audio(void);
void stop_audio(void);
void change_note(unsigned char note);

#endif
