//used to manage buttons and leds and sensors and stuff, nothing LCD related
//TODO:all button check, specific buttons, NEED TO CHECK IF BUTTON WAS RELEASED TOO
#include "io.h"

	DebounceIn Left(LEFT);
	DebounceIn Select(SELECT);
	DebounceIn Right(RIGHT);
	DebounceIn Menu(MENU);

	Pressed leftBtn(Left);
	Pressed selectBtn(Select);
	Pressed rightBtn(Right);
	Pressed menuBtn(Menu);

	DigitalOut myled(D2);

io::io() {

}

void io::init() {
	Left.set_debounce_us(5000);
	Select.set_debounce_us(5000);
	Right.set_debounce_us(5000);
	Menu.set_debounce_us(5000);

	leftPressed = false;
}

bool io::pressSelect() {
	return selectBtn.btnPress();
}

bool io::pressLeft() {
	return leftBtn.btnPress();
}

bool io::pressRight() {
	return rightBtn.btnPress();
}

bool io::pressMenu() {
	return menuBtn.btnPress();
}

bool io::btnPressAll() {
	if(leftBtn.btnPress() || selectBtn.btnPress() || rightBtn.btnPress() || menuBtn.btnPress()) {
		return true;
	}
	else {
		return false;
	}
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
