/*********************************
* "DrivingControl.h"             *
* Reflex detection + run control.*
* Oskar Jentzsch Seeberg         *
*********************************/
#ifndef DRIVINGCONTROL_H
#define DRIVINGCONTROL_H

void initSensor();

void detectRefleks();

void debounceTimer();

void drive();

void setup();

void startCar();

#endif
