/*********************************************
* "uart.h"                                    *
* Header file for the USART0 byte/command UART.*
***********************************************/

#ifndef UART_H_
#define UART_H_

#include <stdint.h>

void UART_Init(unsigned long BaudRate, unsigned char DataBit, unsigned char Rx_Int);
void UART_SendCommand(char* command);
void UART_SendBytes(uint8_t* bytesArr, uint8_t arrLength);

#endif /* UART_H_ */
