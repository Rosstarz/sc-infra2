#include <util/delay.h>
#include <avr/io.h>
#include <usart.h>
#include <display.h>
#include <isr.h>
#include <timer.h>

uint16_t adc_value = 0;

void initADC()
{
    ADMUX |= ( 1 << REFS0 );    //Set up of reference voltage. We choose 5V as reference.
    ADCSRA |= ( 1 << ADPS2 ) | ( 1 << ADPS1 ) | ( 1 << ADPS0 );  //Determine a sample rate by setting a division factor. Used division factor: 128
    ADCSRA |= ( 1 << ADEN ); //Enable the ADC
}

ISR(TIMER0_COMPA_vect)
{
    writeNumber(adc_value);
    blankSegment(3);
}

ISR(TIMER0_OVF_vect)
{
    adc_value = ADC;
}

int main()
{
    initUSART();
    initADC();
    initDisplay();
    initTimerInterrupts(1);
    enableInterrupts();
    printf("\n\n\nPotenciometer on dispay\n\n");
    while (1)
    {
        ADCSRA |= ( 1 << ADSC );    //Start the analog --> digital conversion
        loop_until_bit_is_clear( ADCSRA, ADSC );    //Wait until the conversion is completed
        printf("\rValue: %d", adc_value);
        _delay_ms(100); // Delay for better readability, adjust as needed
        fflush(stdout);
    }

    return 0;
}