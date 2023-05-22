#include <util/delay.h> //This library includes the function named _delay_ms(..) which pauses the execution of the program.
#include <avr/io.h> //Use this library to name the various registers of the ATmega328P microcontroller, like eg DDRB
#include <usart.h> // USB communication library
#include <stdio.h> // Standard input/output library
#include <stdbool.h>
#include <mine.h> 
#include <leds.h> // Library for controlling the LEDs
#include <btns.h> // Library for controlling the buttons


/* In this code we'll use some predefined macros, like _BV and bit_is_clear
 * These macros make the code more readable.
 * They are defined in avr/sfr_defs.h and are included in avr/io.h. */

 #define DEBUG             //if this line is uncommented, debug info will be printed

int main()
{
    initUSART();
    // DDRC &= ~_BV( PC1 );    /* Set up DDR, instead of using ( 1 << PC1 ) we use the
    
    //                          * macro _BV( bit ). (BV stands for "Bit Value") */
    enableButton(1);

    #ifdef DEBUG            // The following code is only printed when DEBUG is defined
    printf( "DDRC: " );
    printBinaryByte( DDRC );
    #endif

    PORTC |= _BV( PC1 );    /* Activate pull-up of C1.
                             * We use the macro _BV( bit ) instead of ( 1 << PC 1 ) */
    #ifdef DEBUG
    printf( "\nPORTC: " );
    printBinaryByte( PORTC );
    #endif

    while ( 1 )
    {
        printf( "\nPINC: " );
        printBinaryByte( PINC );
        if (buttonPushed(1)) /* instead of (!(PINC & ( 1 << PC1 ))) we use the macro bit_is_clear */
        {
            printf( " - Button 1 pressed!\n" );
        }
        else
        {
            #ifdef DEBUG
            printf( " - Button 1 NOT pressed!!\n" );
            #endif
        }
        _delay_ms( 1000 );
    }
    return 0;
}