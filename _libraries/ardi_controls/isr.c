#include <avr/io.h> //Use this library to name the various registers of the ATmega328P microcontroller, like eg DDRB
#include <avr/interrupt.h>

void activateInterrupt(uint8_t port){
    PCICR |= _BV( port );  /* in Pin Change Interrupt Control Register: indicate
                                * which interrupt(s) you want to activate (PCIE0: port B,
                                * PCIE1: port C, PCIE2: port D) */
}

void indicatePCMSK1(uint8_t pin){
    PCMSK1 |= _BV( pin );   /* In the corresponding Pin Change Mask Register: indicate
                                 * which pin(s) of that port activate the ISR. */
}

void enableInterrupts(){
    sei();  // Set Enable Interrupts --> activate the interrupt system globally.
}