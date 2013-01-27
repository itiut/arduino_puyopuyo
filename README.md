# arduino_puyopuyo
ArduinoとRGBLEDを使ったぷよぷよ

## 依存ライブラリ
### tlc5940arduino
1. Tlc5940_xxx.zip を次のページからダウンロード http://code.google.com/p/tlc5940arduino/
2. ライブラリディレクトリ（\<Arduinoをインストールしたディレクトリ\>/libraries/）に解凍する  
例. Linux の場合: /usr/share/arduino/libraries/
3. tlc_config.h 中のドライバICの数を表す定数 `NUM_TLCS` を1から15に変更する

before

    #define NUM_TLCS 1

after

    #define NUM_TLCS 15


### SNESpad
1. SNESpad_xxx.zip を次のページからダウンロード http://code.google.com/p/nespad/
2. ライブラリディレクトリに解凍する
3. Arduino IDEのバージョンが1.0以上なら、SNESpad.cpp 中のインクルードディレクティブを次のように修正する

before

    #include "WProgram.h"

after

    #if defined(ARDUINO) && ARDUINO >= 100
    #include "Arduino.h"
    #else
    #include "WProgram.h"
    #endif


## Requires
### tlc5940arduino
1. Download Tlc5940_xxx.zip from http://code.google.com/p/tlc5940arduino/
2. Unzip to \<Arduino directory\>/libraries/  
e.g. /usr/share/arduino/libraries/ (on Linux)
3. Edit `#define NUM_TLCS 1` to `#define NUM_TLCS 15` in tlc_config.h

### SNESpad
1. Download SNESpad_xxx.zip from http://code.google.com/p/nespad/
2. Unzip to \<Arduino directory\>/libraries/
3. If Arduino IDE version is 1.0 or higher, fix include directive in SNESpad.cpp

before

    #include "WProgram.h"

after

    #if defined(ARDUINO) && ARDUINO >= 100
    #include "Arduino.h"
    #else
    #include "WProgram.h"
    #endif

