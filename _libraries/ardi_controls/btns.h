#include <avr/io.h> //Use this library to name the various registers of the ATmega328P microcontroller, like eg DDRB
#include <util/delay.h> //This library includes the function named _delay_ms(..) which pauses the execution of the program.

void enableButton( int button );
void enableAllButtons();
int buttonPushed( int button );
