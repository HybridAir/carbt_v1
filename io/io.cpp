//used to manage buttons and leds and sensors and stuff, nothing LCD related
//TODO:all button check, specific buttons, idk
#include "io.h"

	DebounceIn Prev(PREV);
	DebounceIn Play(SELECT);
	DebounceIn Next(NEXT);
	DebounceIn Menu(MENU);

	DigitalOut myled(D2);

io::io() {

}

void io::init() {
	Play.set_debounce_us(5000);
}

bool io::btnCheckAll() {
	if(Prev.read() || Play.read() || Next.read() || Menu.read()) {
		return true;
	}
	else {
		return false;
	}
}

void io::led(bool on) {
	myled = on;
}
