ARDUINO := /usr/share/arduino
BOARD := uno
SOURCES := arduino_puyopuyo.ino LED.cpp game.cpp
LIBRARIES := Tlc5940 SNESpad
include ./arduino.mk

.PHONY: check-syntax
INCLUDE := -I/usr/share/arduino/hardware/arduino/cores/arduino
CFLAGS := -Wall -Wextra $(INCLUDE)

check-syntax:
	$(CC) $(CFLAGS) -pedantic -fsyntax-only $(CHK_SOURCES)
