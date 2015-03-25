//controls core lcd stuff, including custom chars, brightness, contrast, etc

#include "Display.h"

extern Serial pc;

Display::Display(TextLCD_SPI_N& lcdin) : lcd(lcdin) {
	//wait_ms(1000);												//short delay so the lcd can initialize itself before we start messing with it
	//pc.printf("got disp\r\n");
};

void Display::init() {
	//create the custom chars
	lcd.setUDC(0, (char *) top1);
	lcd.setUDC(1, (char *) top2);
	lcd.setUDC(2, (char *) left2);
	lcd.setUDC(3, (char *) left1);
	lcd.setUDC(4, (char *) right2);
	lcd.setUDC(5, (char *) right1);
	//lcd.setUDC(6, (char *) pause);
	//lcd.setUDC(7, (char *) play);
	wait_ms(250);
}


//used to clear a specific row, needs the row to clear
void Display::clearRow(int row) {
	lcd.locate(0, row);
	for(int x = 0;x < lcd.columns();x++) {						//for each column in the lcd
		lcd.printf(" ");										//write an empty character
	}
}


//used to center a string of text in a single row, only works well on evenly lengthed strings
void Display::centerText(string in) {
	char length = in.length();
	char space = (lcd.columns() - length) / 2;
	for(int x = 0;x < space;x++) {
		lcd.printf(" ");
	}
	lcd.printf("%s", in.c_str());

}
