//displays and controls the Music Control page
//allows the user to control music playback through the XS3868
//maybe one day it will get song info support :C

#include "PageMusic.h"


//default constructor, needs the lcd and io objects
PageMusic::PageMusic(TextLCD_SPI_N& lcdIn, io& ioIn, Display& dispIn, XS3868& btIn) : bt(btIn), lcd(lcdIn), inout(ioIn), disp(dispIn) {

}


//used to display and run the page, must be called continuously
void PageMusic::showPage(bool first) {
	if(first) {
		lcd.cls();
		lcd.setUDC(0, (char *) pause);
		lcd.setUDC(1, (char *) nextChar);
		lcd.setUDC(2, (char *) prevChar);
		//disp.clearRow(1);
	}

	showControls();
	status();
	btnMon();

}

void PageMusic::status() {

	lcd.locate(0,1);
	if(bt.getSongStatus()) {
		lcd.printf("Playing");
	}
	else {
		lcd.printf("Paused");
	}

}

void PageMusic::btnMon() {
	if(inout.pressSelect()) {
		//play/pause
	}
	else if(inout.pressLeft()) {
		//next song/fastforward
	}
	else if(inout.pressRight()) {
		//prev song/rewind
	}
}

void PageMusic::showControls() {
	lcd.locate(0,0);
	lcd.putc(1);
	lcd.putc(0);
	lcd.printf("  ");
	lcd.putc(1);
	lcd.putc(1);
	lcd.printf("  ");
	lcd.putc(2);
	lcd.putc(2);
	lcd.printf("  Menu");
}
