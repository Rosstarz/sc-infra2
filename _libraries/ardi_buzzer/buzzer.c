#include "buzzer.h"

#include <avr/io.h>
#include <util/delay.h>
#include <string.h>

typedef struct
{
   char tone[2];
   float value;
} TONE;

void enableBuzzer()
{
   DDRD |= (1 << PD3); // Buzzer is connected to PD3
}

float getToneFrequency(char tone[2])
{
   TONE tones[] = {
       {"C5", C5},
       {"D5", D5},
       {"E5", E5},
       {"F5", F5},
       {"G5", G5},
       {"A5", A5},
       {"B5", B5},
       {"C6", C6},
   };
   for (uint8_t i = 0; i < sizeof(tones) / sizeof(TONE); i++)
   {
      if (strcmp(tone, tones[i].tone) == 0)
      {
         return tones[i].value;
      }
   }
   return 0;
}

void playTone(char tone[2], uint32_t duration)
{
   float frequency = getToneFrequency(tone);
   if (frequency == 0)
   {
      return;
   }
   uint32_t periodInMicro = (uint32_t)(1000000 / frequency);              // Calculate the period in microsecs from the freq
   uint32_t durationInMicro = duration * 1000;                            // We express duration in microsecs
   for (uint32_t time = 0; time < durationInMicro; time += periodInMicro) // See tutorial on Music for more info!
   {
      PORTD &= ~(1 << PD3);         // turn the buzzer on
      _delay_us(periodInMicro / 2); // Wait for the half of the period
      PORTD |= (1 << PD3);          // Turn the buzzer off
      _delay_us(periodInMicro / 2); // Wait again for half of the period
   }
}

void playToneFQ(float frequency, uint32_t duration)
{
   if(frequency == 0)
   {
      _delay_ms(duration);
      return;
   }
   uint32_t periodInMicro = (uint32_t)(1000000 / frequency);              // Calculate the period in microsecs from the freq
   uint32_t durationInMicro = duration * 1000;                            // We express duration in microsecs
   for (uint32_t time = 0; time < durationInMicro; time += periodInMicro) // See tutorial on Music for more info!
   {
      PORTD &= ~(1 << PD3);         // turn the buzzer on
      _delay_us(periodInMicro / 2); // Wait for the half of the period
      PORTD |= (1 << PD3);          // Turn the buzzer off
      _delay_us(periodInMicro / 2); // Wait again for half of the period
   }
}