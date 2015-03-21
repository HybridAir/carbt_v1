//controls and manages "pages", and all other lcd content

#include "Pager.h"

extern Serial pc;
//ScrollText search(lcd, "Searching for phone", 6, 1, 8, 200);

Pager::Pager() : searchText(lcd, "Searching", 6, 1, 8, 200),
connectingText(lcd, "Connecting", 6, 1, 8, 200), connectedText(lcd, "Connected", 6, 1, 8, 200),
bterrorText(lcd, "Failed to connect, continue without connecting?", 0, 0, 16, 200), disp(lcd), lcd(D8, D9, D4, D5, D6, D7) {				//default constructor, also initializes Display and TextLCD
	disp.init();														//get the lcd ready for use
	init();

}

void Pager::init() {
	lcd.cls();
	wait_ms(250);
	lcd.locate(1, 0);
	lcd.putc(0x7E);
	lcd.putc(1);
	wait_ms(250);

	lcd.locate(0, 1);
	lcd.putc(3);;
	lcd.putc(2);
	wait_ms(250);

	lcd.putc(4);
	lcd.putc(5);
	wait_ms(250);

	lcd.locate(6, 0);
	lcd.printf("carbt_v1");
	lcd.locate(6, 1);
	lcd.printf("Loading");
}

void Pager::search() {
	searchText.scroll();
}

void Pager::connecting() {
	connectingText.scroll();
}

void Pager::connected() {
	connectedText.scroll();
}

void Pager::bterror() {
	bterrorText.scroll();
}
