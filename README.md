# arduino-puyopuyo
ArduinoとRGBLEDを使ったぷよぷよ

## Requires
### tlc5940arduino
1. Download Tlc5940_xxx.zip from http://code.google.com/p/tlc5940arduino/
2. Unzip to [Arduino directory]/libraries/  
e.g. /usr/share/arduino/libraries/ (on Linux)
3. Edit `#define NUM_TLCS 1` to `#define NUM_TLCS 13` in tlc_config.h
