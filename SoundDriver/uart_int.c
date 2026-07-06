/************************************************
* "uart_int.c":                                 *
* Implementation file for Mega2560 UART driver. *
* Using UART 0.                                 *
* Course-provided driver (Henning Hargaard).    *
*************************************************/
#include <avr/io.h>
#include <stdlib.h>
#include "uart_int.h"

// Target CPU frequency
#define XTAL 16000000
#define F_CPU 16000000UL

/*************************************************************************
UART 0 initialization:
    Asynchronous mode.
    Baud rate = 9600.
    Data bits = 8.
    RX and TX enabled.
    No interrupts enabled.
    Number of Stop Bits = 1.
    No Parity.
*************************************************************************/
void InitUART(unsigned long baudrate, uint8_t databits, unsigned char RX_Int)
{
	// Check for a valid baud rate (300-115200) and data-bit count
	if (baudrate < 300 || baudrate > 115200 || databits < 5 || databits > 8)
	return;

	// Compute the baud rate register value
	uint16_t ubrr_value = (F_CPU / (16UL * baudrate)) - 1;
	UBRR0 = ubrr_value;

	if (RX_Int){
		// If RX_Int is non-zero, enable the RX interrupt as well as the transmitter and receiver
		UCSR0B = (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0);
	} else {
		// Enable transmitter and receiver
	UCSR0B = (1 << RXEN0) | (1 << TXEN0);
	}

	// Select data-bit count (only 5-8 supported)
	UCSR0C = (databits-5) << 1;
}

/*************************************************************************
  Returns 0 (FALSE), if the UART has NOT received a new character.
  Returns value <> 0 (TRUE), if the UART HAS received a new character.
*************************************************************************/
unsigned char CharReady()
{
	// Bit RXC0 in UCSR0A is 1 when a character has been received
	return (UCSR0A & (1 << RXC0));
}

/*************************************************************************
Awaits new character received.
Then this character is returned.
*************************************************************************/
char ReadChar()
{
	// Block while RXC0 is clear, i.e. while no character has been received
	while (!(UCSR0A & (1 << RXC0)))
	{}
	return UDR0;
}

/*************************************************************************
Awaits transmitter register ready.
Then send the character.
Parameter :
	Tegn : Character for sending.
*************************************************************************/
void SendChar(char Tegn)
{
	// UDRE0 in UCSR0A signals that the transmit buffer is ready for a new byte
   while (!(UCSR0A & (1 << UDRE0)))
   {}
	// Once ready, write the character to UDR0
   UDR0 = Tegn;
}

/*************************************************************************
Sends 0 terminated string.
Parameter:
   Streng: Pointer to the string.
*************************************************************************/
void SendString(char* Streng)
{
	// Walk the string until the '\0' terminator
	while(*Streng != '\0'){
		// Send the current character
		SendChar(*Streng);
		// Advance the pointer to the next character
		Streng++;
	}
}

/*************************************************************************
Converts the integer "Tal" to an ASCII string - and then sends this string
using the USART.
Makes use of the C standard library <stdlib.h>.
Parameter:
    Tal: The integer to be converted and sent.
*************************************************************************/
void SendInteger(int Tal)
{
	char array[12];  // Enough room for an int plus a sign and '\0' (max: -2147483648 -> 11 chars)

	itoa(Tal, array, 10);  // Convert the number to a decimal string

	SendString(array);     // Send the string over UART
}

/************************************************************************/
