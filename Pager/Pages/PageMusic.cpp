//Music Control page, allows the user to control the two most commonly used functions, music and fm frequency control
//pressing the menu button switches modes, holding it goes to the menu like usual
//maybe one day it will get song info support :C

#include "PageMusic.h"

extern Serial pc;

//default constructor, needs the lcd and io objects
PageMusic::PageMusic(TextLCD_SPI_N& lcdIn, LcdUtils& utilsIn, io& ioIn, XS3868& btIn) : bt(btIn), lcd(lcdIn), inout(ioIn), utils(utilsIn) {
	musicMode = true;
	prevMusicMode = true;						//used for keeping track of mode changes
	//currentBtn = false;							//keep track of the currently pressed button
	//probably just trigger the title in the button function
}


//used to display and run the page, must be called continuously
void PageMusic::showPage(bool first) {
	if(first) {								//if this page is being shown for the first time (device was just turned on, new page, etc)
		lcd.cls();
	}

	if(btnMon == 4) {						//if the mode button was just pressed
		if(musicMode) {						//if the current mode is music mode
			musicMode = false;				//it will now be fm freq mode
		}
		else {								//vice versa
			musicMode = true;
		}
		lcd.cls();
		//show new mode title
	}

	//status();								//show music and fm freq status on second row, needs to be fixed first

	if(musicMode) {							//if the page is set to music control mode
		showControls(true);
		doMusic();
	}
	else {									//if the page is set to fm freq control mode
		showControls(false);
		doFreq();
	}
}


//handles the music mode operation
void PageMusic::doMusic() {

}


//handles the fm freq mode operation
void PageMusic::doFreq() {

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


//returns the number ID of the currently pressed button
char PageMusic::btnMon() {
	if(inout.pressSelect()) {
		return 1;
	}
	else if(inout.pressLeft()) {
		return 2;
	}
	else if(inout.pressRight()) {
		return 3;
	}
	else if(inout.pressMenu()) {
		return 4;
	}
	else {
		return 0;
	}
}

void PageMusic::showControls(bool music) {
	if(music) {
		lcd.locate(0,0);
		utils.playPauseIcon();
		lcd.printf("  ");
	}
	else {
		lcd.locate(4,0);
	}

	utils.prevIcon();
	lcd.printf("  ");
	utils.nextIcon();
	lcd.printf("  Mode");
}
