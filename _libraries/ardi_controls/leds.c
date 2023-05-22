#include <avr/io.h> //Use this library to name the various registers of the ATmega328P microcontroller, like eg DDRB
#define __DELAY_BACKWARD_COMPATIBLE__  
#include <util/delay.h> //This library includes the function named _delay_ms(..) which pauses the execution of the program.
#include <mine.h>
#include <stdbool.h>

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

bool isLedOn(int i){
    if (bit_is_set( PORTB, PB2 + i ))
        return false;
    else
        return true;
}

void dimLed(int lednumber, int percentage, int duration, bool returnToOriginalState){
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

    if (val == 0 && returnToOriginalState == true)
        lightUpOneLed(lednumber);
    else if (val == 1 && returnToOriginalState == true)
        lightDownOneLed(lednumber);
}

void fadeInLed(int lednumber, int duration){
    int i = 0;
    while (i < 10)
    {
        dimLed(lednumber, i*10, duration/10, false);
        i++;
    }
}

void fadeOutLed(int lednumber, int duration){
    int i = 10;
    while (i > 0)
    {
        dimLed(lednumber, i*10, duration/9, false);
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