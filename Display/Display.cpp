//controls lcd stuff

#include "Display.h"

//TextLCD lcd(D8, D9, D4, D5, D6, D7);  // RS, E, DB4, DB5, DB6, DB7

//Display::Display() : lcd(D8, D9, D4, D5, D6, D7) {
Display::Display() : lcd(D8, D9, D4, D5, D6, D7) {
	lcd.locate(0, 0);
	lcd.printf("Music Control");
}

void Display::init() {

}
