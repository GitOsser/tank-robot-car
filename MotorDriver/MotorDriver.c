/*********************************************
* "MotorDriver.c"                             *
* Implementation file for the motor driver.   *
* Oskar Jentzsch Seeberg                      *
***********************************************/
#include <avr/io.h>
#include "MotorDriver.h"

void initMotor()
{
	// Set PB5 (pin 11) as output, since pin 11 is the output of Timer1 A
	DDRB |= (1 << DDB5);

	// Fast PWM, non-inverting, prescaler 8.
	// Frequency = 16 MHz / 8 / 1024 = 1953.125 Hz, T = 512 us.
	TCCR1A |= (1 << COM1A1) | (1 << WGM11) | (1 << WGM10);
	TCCR1B |= (1 << WGM12) | (1 << CS11);

};

void setBackward(bool Backward)
{
	 // Set Pin 23 as output by setting its direction bit in DDRA to 1.
	 DDRA |= (1 << PIN_23);

	 if (Backward) {
		 // Drive the motor backwards: Pin 23 HIGH
		 PORTA |= (1 << PIN_23);
		 } else {
		 // Drive the motor forwards: Pin 23 LOW
		 PORTA &= ~(1 << PIN_23);
	 }
};

void setMotorIntensity(int intensity)
{

	// Clamp intensity to the 0-100 range
	if (intensity < 0) intensity = 0;
	if (intensity > 100) intensity = 100;

	// Convert to an OCR value
	OCR1A = (intensity * 1023UL) / 100;

};
