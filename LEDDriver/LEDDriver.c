/*********************************************
* "LEDDriver.c"                               *
* Front- and rear-light PWM driver (Timer0).  *
***********************************************/
#include <avr/io.h>
#include "LEDDriver.h"

void initLys(void)
{
	// Front light
	DDRG |= (1 << PG5);
	TCCR0A |= (1 << COM0B1) | (1 << WGM01) | (1 << WGM00);  // Fast PWM, non-inverting
	TCCR0B |= (1 << CS00); // No prescaling
	OCR0B = 0; // Duty cycle = 0, off

	// Rear light
	DDRB |= (1 << PB7);
	TCCR0A |= (1 << COM0A1);  // Fast PWM, non-inverting
	OCR0A = 0; // Duty cycle = 0, off

	// Drive outputs to 0 - lights off
	PORTG &= ~(1 << PG5);
	PORTB &= ~(1 << PB7);
}

void setIntensityFront(int intensity)
{
	if (intensity < 0) intensity = 0;
	if (intensity > 255) intensity = 255;
	OCR0B = intensity;
}

void setIntensityRear(int intensity)
{
	if (intensity < 0) intensity = 0;
	if (intensity > 255) intensity = 255;
	OCR0A = intensity;
}
