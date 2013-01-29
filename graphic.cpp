#include "graphic.h"
#include "LED.h"
#include "Arduino.h"
#include <WString.h>


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

const bool Graphic::A[12][6] = {
	{0,0,0,0,0,0},
	{0,0,0,0,0,0},
	{0,0,0,0,0,0},
	{0,1,1,1,0,0},
	{1,0,0,0,1,0},
	{1,0,0,0,1,0},
	{1,0,0,0,1,0},
	{1,1,1,1,1,0},
	{1,0,0,0,1,0},
	{1,0,0,0,1,0},
	{0,0,0,0,0,0},
	{0,0,0,0,0,0}
};
const bool Graphic::E[12][6] = {
	{0,0,0,0,0,0},
	{0,0,0,0,0,0},
	{0,0,0,0,0,0},
	{1,1,1,1,1,0},
	{1,0,0,0,0,0},
	{1,0,0,0,0,0},

	{1,1,1,1,0,0},
	{1,0,0,0,0,0},
	{1,0,0,0,0,0},
	{1,1,1,1,1,0},
	{0,0,0,0,0,0},
	{0,0,0,0,0,0}
};
const bool Graphic::G[12][6] = {
	{0,0,0,0,0,0},
	{0,0,0,0,0,0},
	{0,0,0,0,0,0},
	{0,0,1,1,1,0},
	{0,1,0,0,0,1},
	{0,1,0,0,0,0},

	{0,1,0,0,0,0},
	{0,1,0,0,1,1},
	{0,1,0,0,0,1},
	{0,0,1,1,1,0},
	{0,0,0,0,0,0},
	{0,0,0,0,0,0}
};
const bool Graphic::M[12][6] = {
	
	{0,0,0,0,0,0},
	{0,0,0,0,0,0},
	{0,0,0,0,0,0},
	{0,1,0,0,0,1},
	{0,1,1,0,1,1},
	{0,1,0,1,0,1},
	{0,1,0,0,0,1},
	{0,1,0,0,0,1},
	{0,1,0,0,0,1},
	{0,1,0,0,0,1},
	{0,0,0,0,0,0},
	{0,0,0,0,0,0},
	
};
const bool Graphic::O[12][6] = {
	{0,0,0,0,0,0},
	{0,0,0,0,0,0},
	{0,0,0,0,0,0},
	{0,1,1,1,0,0},
	{1,0,0,0,1,0},
	{1,0,0,0,1,0},
	{1,0,0,0,1,0},
	{1,0,0,0,1,0},
	{1,0,0,0,1,0},
	{0,1,1,1,0,0},
	{0,0,0,0,0,0},
	{0,0,0,0,0,0}
};
const bool Graphic::R[12][6] = {
	{0,0,0,0,0,0},
	{0,0,0,0,0,0},
	{0,0,0,0,0,0},
	{1,1,1,1,0,0},
	{1,0,0,0,1,0},
	{1,0,0,0,1,0},
	{1,0,0,0,1,0},
	{1,1,1,1,0,0},
	{1,0,0,0,1,0},
	{1,0,0,0,1,0},
	{0,0,0,0,0,0},
	{0,0,0,0,0,0}
};
const bool Graphic::V[12][6] = {
	{0,0,0,0,0,0},
	{0,0,0,0,0,0},
	{0,0,0,0,0,0},
	{1,0,0,0,1,0},
	{1,0,0,0,1,0},
	{1,0,0,0,1,0},
	{1,0,0,0,1,0},
	{1,0,0,0,1,0},
	{0,1,0,1,0,0},
	{0,0,1,0,0,0},
	{0,0,0,0,0,0},
	{0,0,0,0,0,0}
};

void Graphic::PrintChar(LED led, char ch, COLOR color, int slide) {
	
	for (int x = 0; x < led.kWidth; x++){
		for (int y = 0; y < led.kHeight; y++){

			switch(ch){

				case '1':
					if(One[y][x+slide]) led.SetColor(x, y ,color);
					break;
				case '2':
					if(Two[y][x+slide]) led.SetColor(x, y ,color);
					break;
				case '3':
					if(Three[y][x+slide]) led.SetColor(x, y ,color);
					break;
				case 'A':
					if(A[y][x+slide]) led.SetColor(x, y ,color);
					break;
				case 'E':
					if(E[y][x+slide]) led.SetColor(x, y ,color);
					break;
				case 'G':
					if(G[y][x+slide]) led.SetColor(x, y ,color);
					break;
				case 'M':
					if(M[y][x+slide]) led.SetColor(x, y ,color);
					break;
				case 'O':
					if(O[y][x+slide]) led.SetColor(x, y ,color);
					break;
				case 'R':
					if(R[y][x+slide]) led.SetColor(x, y ,color);
					break;
				case 'V':
					if(V[y][x+slide]) led.SetColor(x, y ,color);
					break;
				default:
					break;

			}
		}
	}
}

void Graphic::CountDown(LED led) {

PrintStr(led, "3", GREEN, 100);	
PrintStr(led, "2", YELLOW, 100);	
PrintStr(led, "1", RED, 100);	


}

void Graphic::PrintStr(LED led, char *str, COLOR color, int delayTime){

	int x,y;
	int length = strlen(str); //文字列の長さ

	bool buf[12][6] = {0}; //画面表示用のバッファ


	for (int i = 0; i < length; i++){ //iは文字列の何文字目かを示す
		for (int j = 0; j < led.kWidth; j++){ //jはその文字の何行目かを示す


			//bufの中身を全て左に１行ずつずらす
			for (x = 0; x < led.kWidth -1; x++){
				for (y = 0; y < led.kHeight; y++){
					buf[y][x] = buf[y][x+1];
				}
			}

	 		//bufの右端には新しいデータを入れる
			for (y = 0; y < led.kHeight; y++){			
				switch(str[i]){					
					case '1':
					buf[y][x] = One[y][j];
					break;
					case '2':
					buf[y][x] = Two[y][j];
					break;
					case '3':
					buf[y][x] = Three[y][j];
					break;
					case 'A':
					buf[y][x] = A[y][j];
					break;
					case 'E':
					buf[y][x] = E[y][j];
					break;
					case 'G':
					buf[y][x] = G[y][j];
					break;
					case 'M':
					buf[y][x] = M[y][j];
					break;
					case 'O':
					buf[y][x] = O[y][j];
					break;
					case 'R':
					buf[y][x] = R[y][j];
					break;
					case 'V':
					buf[y][x] = V[y][j];
					break;
					default:
					buf[y][x] = 0;
					break;
				}
			}

			//現在のbufを表示
			led.ClearAll();
			for (x = 0; x < led.kWidth; x++){
				for (y = 0; y < led.kHeight; y++){
					if(buf[y][x]) led.SetColor(x, y ,color);
				}
			}
			led.Update();
			delay(delayTime);

		}
	}


}