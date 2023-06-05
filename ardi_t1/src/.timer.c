#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>
#include <usart.h>
#include <timer.h>

uint32_t counter = 0;



// This ISR runs every time TCNT0 equals the value in the OCRA register
ISR( TIMER0_COMPA_vect )
{
    counter++;
    PORTB &= ~( _BV( PB2 ) | _BV( PB3 ) | _BV( PB4 ) | _BV( PB5 ));
}

// This ISR runs every time TCNT0 equals the TOP value (255)
ISR( TIMER0_OVF_vect )
{
    counter++;
    PORTB |= _BV( PB2 ) | _BV( PB3 ) | _BV( PB4 ) | _BV( PB5 );
}

int main()
{
    initUSART();
    // initTimer0();   //initialize Timer 0
    initTimerInterrupts(3);
    DDRB |= _BV( PB2 ) | _BV( PB3 ) | _BV( PB4 ) | _BV( PB5 );  //Place the LEDs in output mode

    while (1)
    {
        printf( "**********************************************************************************\n" );
        printf( "*********************************** Tick Tock Demo *******************************\n" );
        printf( "**********************************************************************************\n" );

        printf( "Timer 0 is set to increment its TCNT0 register every 64 us up to a maximum of 255.\n" );
        printf( "Observe the LEDs...\n" );

        printf( "Enter the OCR0A value in 3 digit format (000-255): " );

        // setOCR0AValue( getNumber() );
        _delay_ms( 4000 );

        printf( "Timer 0 generates *in the background* continuously two interrupts:\n" );
        printf( "\t*) when the ORC0A (%d) is reached.\n", OCR0A );
        printf( "\t*) when TOP (%d) is reached.\n\n", 255 );

        printf( "Meanwhile we are at a total of %d interrupts.\n\n", counter );
    }

    return 0;
}