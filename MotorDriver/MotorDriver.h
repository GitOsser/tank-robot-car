/*********************************
* "MotorDriver.h"                *
* Header file for the motor driver.*
* Oskar Jentzsch Seeberg         *
*********************************/
#ifndef MOTORDRIVER_H
#define MOTORDRIVER_H

#include <stdbool.h>

// Pin 23 is on PORTA and is bit 1 (direction control)
#define PIN_23 1

void initMotor();
void setBackward(bool Backward);
void setMotorIntensity(int intensity);

#endif
