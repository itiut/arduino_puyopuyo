ARDUINO := /usr/share/arduino
BOARD := uno
SOURCES := arduino_puyopuyo.ino LED.cpp
LIBRARIES := Tlc5940 SNESpad
include ./arduino.mk

.PHONY: check-syntax
check-syntax:
	$(CC) -Wall -Wextra -pedantic -fsyntax-only $(CHK_SOURCES)
