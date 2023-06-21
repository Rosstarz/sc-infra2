#include <avr/io.h> //Use this library to name the various registers of the ATmega328P microcontroller, like eg DDRB
#include <avr/interrupt.h>

void activateInterrupt(uint8_t port);
void indicatePCMSK1(uint8_t pin);
void enableInterrupts();