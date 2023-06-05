#include <avr/io.h>
#include <usart.h>
#define __DELAY_BACKWARD_COMPATIBLE__
#include <util/delay.h>
#include <buzzer.h>

// DURATION IS IN MILLISECONDS
#define DURATION 150

int main()
{
    initUSART();
    // float frequencies[] = { C5, D5, E5, F5, G5, A5, B5, C6 }; //do-re-mi...
    // float frequencies[] = {C5, D5, E5};
    // printf( "Hello, world!\n" );
    enableBuzzer();
    for (int note = 0; note < 1; note++)
    {
        playTone("C5", DURATION);
        // printf("Period: %d\n", (int)frequencies[note]);
        _delay_ms(100);
    }
    return 0;
}