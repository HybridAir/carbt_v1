//displays and controls the Music Control page
//allows the user to control music playback through the XS3868
//maybe one day it will get song info support :C

#include "PageMusic.h"

extern Serial pc;

//default constructor, needs the lcd and io objects
PageMusic::PageMusic(TextLCD_SPI_N& lcdIn, LcdUtils& utilsIn, io& ioIn, XS3868& btIn) : bt(btIn), lcd(lcdIn), inout(ioIn), utils(utilsIn) {
	pc.printf("pmus");
}


//used to display and run the page, must be called continuously
void PageMusic::showPage(bool first) {
	if(first) {
		lcd.cls();
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
	utils.playPauseIcon();
	lcd.printf("  ");
	utils.nextIcon();
	lcd.printf("  ");
	utils.prevIcon();
	lcd.printf("  ");
	lcd.printf("  Menu");
}
