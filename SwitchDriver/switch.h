/************************************
* "Switch.h"                        *
* Header file for SWITCH driver.    *
* Henning Hargaard                  *
************************************/

#ifndef SWITCH_H
#define SWITCH_H

void initSwitchPort();
unsigned char switchStatus();
unsigned char switchOn(unsigned char switch_nr);

#endif
