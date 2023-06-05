#include "leds.h"

#include <avr/io.h> //Use this library to name the various registers of the ATmega328P microcontroller, like eg DDRB
#define __DELAY_BACKWARD_COMPATIBLE__  
#include <util/delay.h> //This library includes the function named _delay_ms(..) which pauses the execution of the program.
#include <mine.h>
// #include <stdbool.h>

void enableOneLed(int i){
    DDRB |= ( 1 << ( PB2 + i ));
}

void enableMultipleLeds(uint8_t i){
    DDRB |= i;
}

void enableAllLeds(){
    for ( int i = 0; i < 4; i++ )
    {
        enableOneLed(i);
    }
}

void lightUpOneLed(int i){
    PORTB &= ~( 1 << ( PB2 + i ));
}

void lightUpMultipleLeds(uint8_t i){
    PORTB |= i;
}

void lightUpMultipleLeds2(int led1, int led2, int led3, int led4){
    if (led1)
        lightUpOneLed(0);
    if (led2)
        lightUpOneLed(1);
    if (led3)
        lightUpOneLed(2);
    if (led4)
        lightUpOneLed(3);
}

void lightUpAllLeds(){
    for ( int i = 0; i < 4; i++ )
    {
        lightUpOneLed(i);
    }
}

void lightDownOneLed(int i){
    PORTB |= ( 1 << ( PB2 + i ));
}

void lightDownMultipleLeds(uint8_t i){
    PORTB &= ~i;
}

void lightDownAllLeds(){
    for ( int i = 0; i < 4; i++ )
    {
        lightDownOneLed(i);
    }
}

void lightToggleOneLed(int i){
    PORTB ^= ( 1 << ( PB2 + i ));
}

void enableAllLedsDown(){
    enableAllLeds();
    lightDownAllLeds();
}

int isLedOn(int i){
    if (bit_is_set( PORTB, PB2 + i ))
        return 0;
    else
        return 1;
}

void dimLed(int lednumber, int percentage, int duration, int returnToOriginalState){
    // dims a led for a certain duration, then returns it to its original state
    int i = 0;
    int val = bitRead( PINB, PB2 + lednumber );

    while (i < (duration / 10))
    {
        lightUpOneLed(lednumber);
        _delay_ms(percentage / 10);
        lightDownOneLed(lednumber);
        _delay_ms((100 - percentage) / 10);
        i++;
    }

    if (!val && returnToOriginalState)
        lightUpOneLed(lednumber);
    else if (val && returnToOriginalState)
        lightDownOneLed(lednumber);
}

void fadeInLed(int lednumber, int duration){
    int i = 0;
    while (i < 10)
    {
        dimLed(lednumber, i*10, duration/10, 0);
        i++;
    }
}

void fadeOutLed(int lednumber, int duration){
    int i = 10;
    while (i > 0)
    {
        dimLed(lednumber, i*10, duration/9, 0);
        i--;
    }
}

void blinkLed(int lednumber, int duration){
    int i = 0;
    while (i < (duration / 10))
    {
        lightToggleOneLed(lednumber);
        _delay_ms(10);
        i++;
    }
}

void blinkAllLeds(int duration){
    int i = 0;
    while (i < (duration / 10))
    {
        lightToggleOneLed(0);
        lightToggleOneLed(1);
        lightToggleOneLed(2);
        lightToggleOneLed(3);
        _delay_ms(10);
        i++;
    }
}