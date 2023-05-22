#include <util/delay.h> //This library includes the function named _delay_ms(..) which pauses the execution of the program.
#include <avr/io.h> //Use this library to name the various registers of the ATmega328P microcontroller, like eg DDRB
#include <usart.h> // USB communication library
#include <stdio.h> // Standard input/output library
#include <mine.h> 
#include <leds.h> // Library for controlling the LEDs

// int main() {
//     initUSART ();
   
//     DDRB = 0b00010000; //Write a binary number to the Data Direction Register B. Pin 5 of port B is enabled for writing with this setting.
//     //DDRB |= 0b00100000; //We could also do a bitwise OR, so that we don't erase ones on other positions of DDRB
//     //DDRB |= (1 << 5); //We could also use the << bitshift operator.
//     //DDRB |= (1 << (PB2 + 3)); //Here we use the port names from io.h to make the code more readable.

//     while (1)
//     {
//         PORTB = (0 << (PB3 + 1)); //We write a 0 to PB2+3 to let the connected LED light up.
//         _delay_ms(1000); //We use the _delay_ms(..) function from delay.h to pause the execution during 1000 milliseconds (1 sec).
//         PORTB = (1 << (PB3 + 1)); //We write a 1 to PB2+3 to let the connected LED go dark.
//         _delay_ms(1000);
//     }

//     return 0;
// }

#define NUMBER_OF_LEDS 4 //Define is a "preprocessor directive". It ensures that every NUMBER_OF_LEDS will be replaced by 4 in the following code

void enableLed ( int lednumber ) //C has no classes; functions can be included directly in the .c file.
{
    if ( lednumber < 0 || lednumber > NUMBER_OF_LEDS-1 ) return;
    DDRB |= ( 1 << ( PB2 + lednumber ));    //Check the tutorial "Writing to a Pin". We know from the documentation on
                                            //the multifunctional shield that the LEDs start at PB2
}

// void lightUpLed ( int lednumber )    //Note: enabled LEDs light up immediately ( 0 = on )
// {
//     if ( lednumber < 0 || lednumber > NUMBER_OF_LEDS-1 ) return;
//     PORTB &= ~( 1 << ( PB2 + lednumber ));  //Check the tutorial on "Bit Operations" to know what happens in this line.
// }

// void lightDownLed ( int lednumber )
// {
//     if ( lednumber < 0 || lednumber > 3 ) return;
//     PORTB |= ( 1 << ( PB2 + lednumber ));   //Make sure you understand this line as well!
// }

int main()
{
    initUSART ();

    int total;
    // calling function from library
    total = addition (10, 20);
    printf ("Total = %d \n", total);

    // enableAllLeds();
    enableMultipleLeds(0b00011100);
    _delay_ms( 2000 );
    // lightDownAllLeds();
    while (1)
    {
        lightUpMultipleLeds(0b00010000);
        // lightToggleOneLed(0);
        _delay_ms( 500 );
        // lightToggleOneLed(0);
        lightDownMultipleLeds(0b00010000);
        _delay_ms( 500 );
        // lightUpOneLed(0);
        // _delay_ms( 500 );
    //     printf("Hello World!\n");
    //     for ( int i = 0; i < 4; i++ )
    //     {
    //         lightDownLed(i);
    //         _delay_ms( 100 );
    //     }
    //     for ( int i = 0; i < 4; i++ )
    //     {
    //         lightUpLed(i);
    //         _delay_ms( 100 );
    //     }
    }
    return 0;
}