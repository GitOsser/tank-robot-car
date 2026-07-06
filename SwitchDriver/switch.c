/************************************
* "switch.c"                        *
* Implementation for SWITCH driver. *
* Course-provided driver            *
* (Henning Hargaard).               *
************************************/
#include <avr/io.h>
#include "switch.h"

// Configures the switch port as input
void initSwitchPort()
{
	DDRA = 0;
}

// Reads all switches at once
unsigned char switchStatus()
{
	return ~PINA;
}

// Returns TRUE if the switch with number
// "switch_nr" is active - otherwise returns FALSE
unsigned char switchOn(unsigned char switch_nr)
{
	if(switch_nr > 7){
		return 0;
		}
	if((PINA & (1 << switch_nr)) == 0 ){
		return 1;
	}
	
		return 0;
	
}