//controls core lcd stuff, including custom chars, brightness, contrast, etc

#include "Display.h"

extern Serial pc;

Display::Display(TextLCD& lcdin) : lcd(lcdin) {
	wait_ms(250);												//short delay so the lcd can initialize itself before we start messing with it
	pc.printf("got disp\r\n");
};

void Display::init() {
	//wait(1);
	//create the custom chars
	lcd.setUDC(0, (char *) top1);
	lcd.setUDC(1, (char *) top2);
	lcd.setUDC(2, (char *) left2);
	lcd.setUDC(3, (char *) left1);
	lcd.setUDC(4, (char *) right2);
	lcd.setUDC(5, (char *) right1);
}


//used to clear a specific row, needs the row to clear
void Display::clearRow(int row) {
	lcd.locate(0, row);
	for(int x = 0;x < lcd.columns();x++) {						//for each column in the lcd
		lcd.printf(" ");										//write an empty character
	}
}
