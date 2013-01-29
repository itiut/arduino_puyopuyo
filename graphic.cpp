#include "graphic.h"
#include "LED.h"
#include "arduino.h"


const bool Graphic::One[12][6] = {
	{0,0,0,0,0,0},
	{0,0,0,1,0,0},
	{0,0,1,1,0,0},
	{0,1,0,1,0,0},
	{0,0,0,1,0,0},
	{0,0,0,1,0,0},
	{0,0,0,1,0,0},
	{0,0,0,1,0,0},
	{0,0,0,1,0,0},
	{0,0,0,1,0,0},
	{0,1,1,1,1,0},
	{0,0,0,0,0,0}
};

const bool Graphic::Two[12][6] = {
	{0,0,0,0,0,0},
	{0,0,1,1,0,0},
	{0,1,0,0,1,0},
	{0,1,0,0,1,0},
	{0,0,0,0,1,0},
	{0,0,0,0,1,0},
	{0,0,0,1,0,0},
	{0,0,0,1,0,0},
	{0,0,1,0,0,0},
	{0,1,0,0,0,0},
	{0,1,1,1,1,0},
	{0,0,0,0,0,0}
};

const bool Graphic::Three[12][6] = {
	{0,0,0,0,0,0},
	{0,0,1,1,0,0},
	{0,1,0,0,1,0},
	{0,1,0,0,1,0},
	{0,0,0,0,1,0},
	{0,0,0,0,1,0},
	{0,0,1,1,0,0},
	{0,0,0,0,1,0},
	{0,1,0,0,1,0},
	{0,1,0,0,1,0},
	{0,0,1,1,0,0},
	{0,0,0,0,0,0}
};


void Graphic::CountDown(LED led) {

	led.ClearAll();
	for (int x = 0; x < led.kWidth; x++){
		for (int y = 0; y < led.kHeight; y++){
			if(Three[y][x]){
				led.SetColor(x, y ,CYAN);
			}
		}
	}
	led.Update();
	delay(1000);
	
	led.ClearAll();
	for (int x = 0; x < led.kWidth; x++){
		for (int y = 0; y < led.kHeight; y++){
			if(Two[y][x]){
				led.SetColor(x, y ,YELLOW);
			}
		}
	}
	led.Update();
	delay(1000);


	led.ClearAll();
	for (int x = 0; x < led.kWidth; x++){
		for (int y = 0; y < led.kHeight; y++){
			if(One[y][x]){
				led.SetColor(x, y ,MAGENTA);
			}
		}
	}
	led.Update();
	delay(1000);

}


