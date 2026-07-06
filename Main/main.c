/*********************************************
* "main.c"                                    *
* Program entry point.                        *
* Oskar Jentzsch Seeberg                      *
***********************************************/

#include <avr/delay.h>
#include <avr/io.h>
#include "../DrivingControl/DrivingControl.h"
#include "../SwitchDriver/switch.h"



int main(void)
{
	setup();
	while(1)
	{
		if(switchOn(7))
		{
			startCar();
		}
	//drive();
	}
}
