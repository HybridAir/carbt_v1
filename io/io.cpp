//used to manage buttons and leds and sensors and stuff, nothing LCD related
//TODO:all button check, specific buttons, idk
#include "io.h"

	DebounceIn Left(LEFT);
	DebounceIn Select(SELECT);
	DebounceIn Right(RIGHT);
	DebounceIn Menu(MENU);

	DigitalOut myled(D2);

io::io() {

}

void io::init() {
	Left.set_debounce_us(5000);
	Select.set_debounce_us(5000);
	Right.set_debounce_us(5000);
	Menu.set_debounce_us(5000);
}

bool io::readLeft() {
	return Left.read();
}

bool io::readSelect() {
	return Select.read();
}

bool io::readRight() {
	return Right.read();
}

bool io::readMenu() {
	return Menu.read();
}

bool io::btnReadAll() {
	if(Left.read() || Select.read() || Right.read() || Menu.read()) {
		return true;
	}
	else {
		return false;
	}
}

void io::led(bool on) {
	myled = on;
}
