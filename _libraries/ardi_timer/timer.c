#include "timer.h"

#include <avr/io.h>
#include <avr/interrupt.h>
// #include <math.h>

// #include <usart.h>

void initTimer0Def()
{
    // STEP 1: choose the WAVE FORM and by consequence the Mode of Operation
    // We choose FAST PWM; TCNT0 counts to 255
    TCCR0A |= _BV( WGM00 ) | _BV( WGM01 );  // WGM00 = 1 and WGM01 = 1 --> Fast PWM Mode

    // STEP 2: *always* set a PRESCALER, otherwise the timer won't count
    // The counting speed is determined by the CPU clock (16 Mhz) divided by this factor
    TCCR0B |= _BV( CS02 ) | _BV( CS00 );    // CS02 = 1 and CS00 = 1 --> prescaler factor is now 1024 (= every 64 us)

    // STEP 3: enable INTERRUPTS
    // Enable interrupts for 2 cases: TCNT0 == TOP and TCNT0 == OCR0A
    TIMSK0 |= _BV( TOIE0 ); // enable overflow interrupt
    TIMSK0 |= _BV( OCIE0A );    // enable OCRA interrupt

    // sei();  // enable interrupts globally
}

void initTimer0(int prescaler)
{
    // STEP 1: choose the WAVE FORM and by consequence the Mode of Operation
    // We choose FAST PWM; TCNT0 counts to 255
    TCCR0A |= _BV( WGM00 ) | _BV( WGM01 );  // WGM00 = 1 and WGM01 = 1 --> Fast PWM Mode

    // STEP 2: *always* set a PRESCALER, otherwise the timer won't count
    // The counting speed is determined by the CPU clock (16 Mhz) divided by this factor
    if (prescaler == 64)
    {
        TCCR0B |= _BV( CS01 ) | _BV( CS00 );    // CS01 = 1 and CS00 = 1 --> prescaler factor is now 64 (= every 4 us)
    }
    else if (prescaler == 256)
    {
        TCCR0B |= _BV( CS02 );    // CS02 = 1 --> prescaler factor is now 256 (= every 16 us)
    }
    else if (prescaler == 1024)
    {
        TCCR0B |= _BV( CS02 ) | _BV( CS00 );    // CS02 = 1 and CS00 = 1 --> prescaler factor is now 1024 (= every 64 us)
    }
    else
    {
        return;
    }

    // STEP 3: enable INTERRUPTS
    // Enable interrupts for 2 cases: TCNT0 == TOP and TCNT0 == OCR0A
    TIMSK0 |= _BV( TOIE0 ); // enable overflow interrupt
    TIMSK0 |= _BV( OCIE0A );    // enable OCRA interrupt

    // sei();  // enable interrupts globally
}

void initTimerInterrupts(int ms)
{
    if (ms < 1 || ms > MAX_INTERRUPT_FREQUENCY)
    {
        return;
    }
    float prescaler = 1024;
    if (ms == 1)
    {
        prescaler = 64;
    }
    else if (ms > 1 && ms < 5)
    {
        prescaler = 256;
    }
    // printf("\nPrescaler:");
    // printf("%d\n", (int) prescaler);
    // printf("CPU clock: %d\n", (int) CPU_CLOCK);
    // float test = (float) ((float) CPU_CLOCK / prescaler);
    // printf("Test: %d\n", (int) test);
    float timer_speed = (1 / (CPU_CLOCK / prescaler));
    // printf("Timer speed: %d\n", (int) timer_speed);
    int ocra_value = (int) ((ms * 1000 / timer_speed) - 1);
    // printf("OCR0A value: %d\n", ocra_value);
    initTimer0(prescaler);
    setOCR0AValue(ocra_value);
}

void setOCR0AValue(uint8_t value)
{
    OCR0A = value;
}
