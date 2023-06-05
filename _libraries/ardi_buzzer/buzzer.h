#include <avr/io.h>
#define __DELAY_BACKWARD_COMPATIBLE__
#include <util/delay.h>
#include <string.h>

// FREQUENCIES OF THE NOTES
#define PAUSE 0
#define G3 196.00
#define Gb3 207.650
#define Ab3 233.080
#define C 261.630
#define D 293.670
#define Db 311.130
#define E 329.630
#define F 349.230
#define G 392.00
#define Gb 415.310
#define Ab 440.00
#define C5 523.250
#define Db5 622.250
#define D5 587.330
#define E5 659.260
#define F5 698.460
#define Gb5 830.610
#define G5 783.990
#define Ab5 932.330
#define A5 880.00
#define B5 987.770
#define C6 1046.500
#define C7 1569.750

// TEMPO
#define DL10 500
#define DL5 250
#define DL2 100
#define DL1 100



void enableBuzzer();
float getToneFrequency(char tone[2]);
void playTone(char tone[2], uint32_t duration);
void playToneFQ(float frequency, uint32_t duration);