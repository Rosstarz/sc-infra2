#include <util/delay.h> //This library includes the function named _delay_ms(..) which pauses the execution of the program.
#include <avr/io.h> //Use this library to name the various registers of the ATmega328P microcontroller, like eg DDRB
#include <usart.h> // USB communication library
#include <stdio.h> // Standard input/output library
#include <mine.h> 
#include <leds.h> // Library for controlling the LEDs

// #define val 0

int main()
{
    initUSART ();

    // int val;
    // val = bitRead( PINB, PB2 );

    // printf("Val = %d\n", bitRead( PINB, PB2 ));
    enableAllLedsDown();

    while (1)
    {
        fadeInLed(0, 2000);
        fadeOutLed(0, 2000);
    }

    // printf("Val = %d\n", bitRead( PINB, PB2 ));
    // dimLed(0, 10, 2000);
    // _delay_ms(200);
    // printf("Val = %d\n", bitRead( PINB, PB2 ));

    return 0;
}