#include <avr/io.h> //Use this library to name the various registers of the ATmega328P microcontroller, like eg DDRB
#define __DELAY_BACKWARD_COMPATIBLE__  
#include <util/delay.h> //This library includes the function named _delay_ms(..) which pauses the execution of the program.
#include <mine.h>
// #include <stdbool.h>


void enableOneLed(int);
void enableMultipleLeds(uint8_t);
void enableAllLeds();

void lightUpOneLed(int); 
void lightUpMultipleLeds(uint8_t);
void lightUpMultipleLeds2(int led1, int led2, int led3, int led4);
void lightUpAllLeds();

void lightDownOneLed(int); 
void lightDownMultipleLeds(uint8_t);
void lightDownAllLeds();

void lightToggleOneLed(int);
void enableAllLedsDown();

int isLedOn(int);

void dimLed(int lednumber, int percentage, int duration, int returnToOriginalState);
void fadeInLed(int lednumber, int duration);
void fadeOutLed(int lednumber, int duration);

void blinkLed(int lednumber, int duration);
void blinkAllLeds(int duration);