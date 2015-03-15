//used to manage buttons and leds and sensors and stuff, nothing LCD related
#include "io.h"

	DigitalIn Prev(PREV);
	DigitalIn Play(SELECT);
	DigitalIn Next(NEXT);
	DigitalIn Menu(MENU);
	DigitalOut myled(D2);

io::io() {



	//myled = 1;

}

void io::doled() {
	myled = 1;
}
