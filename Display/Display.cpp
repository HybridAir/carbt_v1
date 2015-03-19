//controls lcd stuff

#include "Display.h"

//TextLCD lcd(D8, D9, D4, D5, D6, D7);  // RS, E, DB4, DB5, DB6, DB7

Display::Display() : lcd(D8, D9, D4, D5, D6, D7) {

	//create the custom chars
	lcd.setUDC(0, (char *) top1);
	lcd.setUDC(1, (char *) top2);
	lcd.setUDC(2, (char *) left2);
	lcd.setUDC(3, (char *) left1);
	lcd.setUDC(4, (char *) right2);
	lcd.setUDC(5, (char *) right1);

	lcd.locate(1, 0);
	lcd.putc(0);
	lcd.putc(1);

	lcd.locate(0, 1);
	lcd.putc(3);
	lcd.putc(2);
	lcd.putc(4);
	lcd.putc(5);

	lcd.locate(6, 0);
	lcd.printf("carbt_v1");
	lcd.locate(6, 1);
	lcd.printf("ar1");





}

void Display::init() {

}
