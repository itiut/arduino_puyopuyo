UNAME := $(shell uname)

ifeq ($(UNAME),Darwin) #MacOSX
	ARDUINO := /Applications/Arduino.app/Contents/Resources/Java
	ARDUINODIR := $(ARDUINO)
else #Linux
	ARDUINO := /usr/share/arduino
endif


BOARD := uno
SOURCES := arduino_puyopuyo.ino LED.cpp game.cpp graphic.cpp
LIBRARIES := Tlc5940 SNESpad
include ./arduino.mk

.PHONY: check-syntax

INCLUDE := -I$(ARDUINO)/hardware/arduino/cores/arduino
CFLAGS := -Wall -Wextra $(INCLUDE)

check-syntax:
	$(CC) $(CFLAGS) -pedantic -fsyntax-only $(CHK_SOURCES)
