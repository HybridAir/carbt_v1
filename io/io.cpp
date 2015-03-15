//used to manage buttons and leds and sensors and stuff, nothing LCD related
#include "io.h"

io::io() {
	DigitalIn Prev(PREV);
	DigitalIn Select(SELECT);
	DigitalIn Next(NEXT);
	DigitalIn Menu(MENU);
}
