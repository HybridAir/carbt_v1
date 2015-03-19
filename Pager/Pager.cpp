//controls and manages "pages", and all other lcd content

#include "Pager.h"

extern Serial pc;

Pager::Pager() : disp(lcd), lcd(D8, D9, D4, D5, D6, D7){
	disp.init();					//get the lcd ready
	init();

}

void Pager::init() {
	pc.printf("got init\r\n");

	lcd.cls();
	wait_ms(250);
	lcd.locate(1, 0);
	lcd.putc(0);
	lcd.putc(1);
	wait_ms(250);

	lcd.locate(0, 1);
	lcd.putc(3);
	lcd.putc(2);
	wait_ms(250);

	lcd.putc(4);
	lcd.putc(5);


	wait_ms(250);
	lcd.locate(6, 0);
	lcd.printf("carbt_v1");
	lcd.locate(6, 1);
	lcd.printf("Searching for device");				//scroll this
}
