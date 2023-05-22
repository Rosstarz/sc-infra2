#include <avr/io.h> //Use this library to name the various registers of the ATmega328P microcontroller, like eg DDRB
#include <stdbool.h>
#include <util/delay.h> //This library includes the function named _delay_ms(..) which pauses the execution of the program.

void enableButton( int button ){
    DDRB |= (1 << button);
    PORTC |= (1 << button);
}

void enableAllButtons(){
    enableButton( PC1 );
    enableButton( PC2 );
    enableButton( PC3 );
}

bool buttonPushed( int button ){
    if (bit_is_clear( PINC, button )){
        _delay_ms( 100 );
        if ( bit_is_clear( PINC, button )){
            return true;
        }
    }
    return false;
}

// bool buttonReleased( int button ){

// }