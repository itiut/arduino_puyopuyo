#ifndef _GRAPHIC_H_
#define _GRAPHIC_H_

#include "LED.h"

class Graphic {
private:
	static const bool One[12][6];
	static const bool Two[12][6];
	static const bool Three[12][6];


public:
	void CountDown(LED led);


};

#endif /* _GRAPHIC_H_ */
