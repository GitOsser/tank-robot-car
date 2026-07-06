/*********************************************
* "SoundDriver.c"                             *
* DFPlayer-style MP3 module driver over UART. *
***********************************************/
#include <avr/io.h>
#include "uart_int.h"
#include "SoundDriver.h"  // Include your sound.h file
#define F_CPU 16000000UL
#include <util/delay.h>



void initSound(void) {
InitUART(9600, 8, 0);  // Initialize UART at 9600 baud rate
_delay_ms(3000);  // Wait for boot-up

unsigned int checksum;
unsigned char checksumHigh, checksumLow;

// Set volume
unsigned char setVolume[8] = {0x7E, 0x06, 0x00, 0x00, 0x1E, 0x00, 0x00, 0xEF};

// Checksum calculation for volume command
unsigned int sum = setVolume[1] + setVolume[2] + setVolume[3] + setVolume[4];
checksum = 0xFFFF - sum + 1;
checksumHigh = (checksum >> 8) & 0xFF;
checksumLow = checksum & 0xFF;
setVolume[5] = checksumHigh;
setVolume[6] = checksumLow;

// Send volume command
for (uint8_t i = 0; i < 8; i++) {
SendChar(setVolume[i]);
}
_delay_ms(500);  // Wait for the command to take effect
}

void PlaySound(int sound) {

// Get ready for volume, checksum and playing
unsigned char playTrack[8] = {0x7E, 0x03, 0x00, 0x00, 0x01, 0x00, 0x00, 0xEF};
unsigned int checksum;
unsigned char checksumHigh, checksumLow;
unsigned char start = 1;
unsigned char refleks = 2;
unsigned char slut = 3;

if (sound == 1) {  // Assuming '1' corresponds to start
playTrack[4] = 0x01;  // Set start sound
}
else if (sound == 2) {  // Assuming '2' corresponds to refleks
playTrack[4] = 0x02;  // Set refleks sound
}
else if (sound == 3) {  // Assuming '3' corresponds to slut
playTrack[4] = 0x03;  // Set slut sound
}

// Calculate checksum for the playTrack array
unsigned int sum = playTrack[1] + playTrack[2] + playTrack[3] + playTrack[4];
checksum = 0xFFFF - sum + 1;
checksumHigh = (checksum >> 8) & 0xFF;
checksumLow = checksum & 0xFF;
playTrack[5] = checksumHigh;
playTrack[6] = checksumLow;

// Send playTrack command
for (uint8_t i = 0; i < 8; i++) {
SendChar(playTrack[i]);
}
}