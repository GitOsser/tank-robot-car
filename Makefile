# TANK! firmware — ATmega2560 (Arduino Mega 2560), bare-metal C
#
#   make            build tank.hex
#   make flash      upload to the board (override PORT=... as needed)
#   make clean      remove build artifacts

MCU        = atmega2560
F_CPU      = 16000000UL
TARGET     = tank

# Upload settings (Arduino Mega 2560 bootloader)
PROGRAMMER = wiring
PORT       = COM3
UPLOAD_BAUD= 115200

CC       = avr-gcc
OBJCOPY  = avr-objcopy
SIZE     = avr-size

CPPFLAGS = -I. -DF_CPU=$(F_CPU) -mmcu=$(MCU)
CFLAGS   = -Os -Wall -std=gnu11 -ffunction-sections -fdata-sections
LDFLAGS  = -mmcu=$(MCU) -Wl,--gc-sections

SRC := \
	Main/main.c \
	DrivingControl/DrivingControl.c \
	DrivingControl/uart.c \
	MotorDriver/MotorDriver.c \
	LEDDriver/LEDDriver.c \
	SoundDriver/SoundDriver.c \
	SoundDriver/uart_int.c \
	SwitchDriver/switch.c

OBJ := $(SRC:.c=.o)

all: $(TARGET).hex

%.o: %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(TARGET).elf: $(OBJ)
	$(CC) $(LDFLAGS) $^ -o $@
	$(SIZE) --format=avr --mcu=$(MCU) $@

$(TARGET).hex: $(TARGET).elf
	$(OBJCOPY) -O ihex -R .eeprom $< $@

flash: $(TARGET).hex
	avrdude -c $(PROGRAMMER) -p $(MCU) -P $(PORT) -b $(UPLOAD_BAUD) -D -U flash:w:$<:i

clean:
	rm -f $(OBJ) $(TARGET).elf $(TARGET).hex

.PHONY: all flash clean
