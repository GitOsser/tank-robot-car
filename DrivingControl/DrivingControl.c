/*********************************************
* "DrivingControl.c"                          *
* Reflex detection + choreographed run logic. *
* Oskar Jentzsch Seeberg                      *
***********************************************/

#define F_CPU 16000000UL
#include <avr/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>

#include "DrivingControl.h"
#include "../MotorDriver/MotorDriver.h"
#include "../LEDDriver/LEDDriver.h"
#include "../SoundDriver/SoundDriver.h"
#include "../SwitchDriver/switch.h"
#include "uart.h"

#define DEBOUNCE_TICKS 31250 // 2 seconds at prescaler 1024
unsigned int RefleksCount = 100;
/*
Overall logic: detect the reflective track markers. The physical reflex module drives HIGH.
Pin 20 (INT1) and Pin 21 (INT0) are wired up and trigger on a rising edge, which fires an
interrupt (ISR(INT0) / ISR(INT1)). To make sure the same marker is not counted several times,
the interrupt disables itself and starts a debounce timer (Timer3). After 2 seconds a new
compare-match interrupt stops the debounce timer and re-enables INT0 and INT1.
*/

void debounceTimer () {
	TCCR3B |= (1 << WGM32);										// CTC mode (WGM32 = 1)
	TCCR3B |= (1 << CS32) | (1 << CS30);						// Prescaler = 1024

	OCR3A = 15625;												// Time window before a new reflex can be detected
	TCNT3 = 0;

	TIMSK3 |= (1 << OCIE3A);									// Enable output compare interrupt A
}


void initSensor() {
	EICRA |= (1 << ISC01);										// ISC01 = 1, ISC00 = 1: rising edge on INT0
	//EICRA &= ~(1 << ISC00);
	EICRA |= (1 << ISC00);
	EICRA |= (1 << ISC11);										// ISC11 = 1, ISC10 = 1: rising edge on INT1
	//EICRA &= ~(1 << ISC10);
	EICRA |= (1 << ISC10);

	EIMSK |= (1 << INT0);										// Enable INT0
	EIMSK |= (1 << INT1);										// Enable INT1
}


void detectRefleks(){											// Handles the logic when a reflex is detected
																// Temporarily disable both INT0 and INT1
	EIMSK &= ~(1 << INT0);
	EIMSK &= ~(1 << INT1);

	RefleksCount++;
																// Start the debounce timer
	debounceTimer();

	drive();
}

ISR(INT0_vect) {
	detectRefleks();
}

ISR(INT1_vect) {
	detectRefleks();
}

ISR(TIMER3_COMPA_vect) {
	TCCR3B = 0;													// Stop Timer3
	TIMSK3 &= ~(1 << OCIE3A);									// Disable compare match

	EIFR |= (1 << INTF0);										// Clear pending INT0 flag
	EIFR |= (1 << INTF1);
	EIMSK |= (1 << INT0);										// Re-enable INT0
	EIMSK |= (1 << INT1);
}

																// Initializes everything needed for a run

void setup(){
	initSensor();
	initMotor();
	UART_Init(9600, 8, 0);
	_delay_ms(1000);
	initSound();
	initLys();
	initSwitchPort();
	sei();
}



void drive(){

	switch(RefleksCount){
		case 0:
		PlaySound(1);
		_delay_ms(4000);
		setBackward(0);
		setMotorIntensity(60);
		setIntensityFront(215);
		setIntensityRear(27);
		break;

		case 1:
		PlaySound(2);
		break;

		case 2:
		PlaySound(2);
		setMotorIntensity(100);
		break;

		case 3:
		PlaySound(2);
		setMotorIntensity(30);
		break;

		case 4:
		PlaySound(2);
		setMotorIntensity(35);

		break;
		case 5:
		PlaySound(2);
		setMotorIntensity(60);

		break;
		case 6:
		PlaySound(2);
		_delay_ms(300);
		setMotorIntensity(0);
		_delay_ms(500);
		setBackward(1);
		setMotorIntensity(60);
		setIntensityRear(135);

		break;
		case 7:
		PlaySound(2);
		setMotorIntensity(60);

		break;
		case 8:
		PlaySound(2);
		_delay_ms(300);
		setMotorIntensity(0);
		setIntensityRear(27);
		_delay_ms(500);
		setBackward(0);
		setMotorIntensity(60);


		break;
		case 9:
		PlaySound(2);

		break;
		case 10:
		PlaySound(2);

		break;
		case 11:
		_delay_ms(1100);
		setMotorIntensity(0);
		PlaySound(3);

		default:
		setMotorIntensity(0);
		setIntensityFront(0);
		setIntensityRear(0);
		break;
	}
}

void startCar()
{
	RefleksCount = 0;
	drive();
}
