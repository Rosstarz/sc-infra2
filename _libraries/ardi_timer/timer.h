#include <avr/io.h>
#include <avr/interrupt.h>
#include <math.h>

#define CPU_CLOCK 16.000
#define MAX_INTERRUPT_FREQUENCY 16
#define MIN_OCR0A_VALUE 0
#define MAX_OCR0A_VALUE 255


void initTimer0Def();
void initTimer0();
void initTimerInterrupts(int ms);
void setOCR0AValue(uint8_t value);
