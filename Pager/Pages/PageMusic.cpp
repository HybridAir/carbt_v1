//Music Control page, allows the user to control the two most commonly used functions, music and fm frequency control
//pressing the menu button switches modes, holding it goes to the menu like usual
//maybe one day it will get song info support :C
//TODO: implement menu button holding to switch pages

#include "PageMusic.h"

extern Serial pc;


//default constructor, needs the lcd and io objects
PageMusic::PageMusic(TextLCD_SPI_N& lcdIn, LcdUtils& utilsIn, io& ioIn, XS3868& btIn) : bt(btIn), lcd(lcdIn), inout(ioIn), utils(utilsIn) {
	musicMode = true;
	prevMusicMode = true;						//used for keeping track of mode changes
	songStatus = 0;
	songStatTimer.start();
}


//used to display and run the page, must be called continuously
void PageMusic::showPage(bool first) {
	if(first) {								//if this page is being shown for the first time (device was just turned on, new page, etc)
		lcd.cls();
		lcd.locate(0,1);
		lcd.printf("Stopped");
	}

	if(btnMon() == 4) {						//if the mode button was just pressed
		utils.clearRow(0);
		lcd.locate(0,0);

		if(musicMode) {						//if the current mode is music mode
			musicMode = false;				//it will now be fm freq mode
			utils.centerText("FM Mode");
		}
		else {								//vice versa
			musicMode = true;
			utils.centerText("Music Mode");
		}

		wait(1);
		utils.clearRow(0);
	}

	//status();								//show music and fm freq status on bottom row
	showControls();							//show controls on the top row

	if(musicMode) {							//if the page is set to music control mode
		doMusic();
	}
	else {									//if the page is set to fm freq control mode
		doFreq();
	}
}


//handles the music mode operation
void PageMusic::doMusic() {
	if(btnMon() == 1) {					//if the play/pause button was pressed
		utils.clearRow(1);
		lcd.locate(0,1);

		if(bt.playPause()) {
			lcd.printf("Playing");
		}
		else {
			lcd.printf("Paused");
		}
	}
}


//handles the fm freq mode operation
void PageMusic::doFreq() {

}


//displays the currently music status and fm frequency on the buttom row
void PageMusic::status() {
	lcd.locate(0,1);
	if(songStatTimer.read_ms() >= 500) {
		songStatus = bt.getSongStatus();
		songStatTimer.reset();
	}

	if(songStatus == 1) {
		lcd.printf("Playing");
	}
	else if(songStatus == 2) {
		lcd.printf("Paused");
	}

	//leave the default power on status blank I guess

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


//shows different button controls depending on the current mode
void PageMusic::showControls() {
	lcd.locate(0,0);

	if(musicMode) {								//if music mode
		utils.playPauseIcon();
		lcd.printf("  ");
		utils.prevIcon();
		lcd.printf("  ");
	}
	else {										//if fm freq mode
		utils.prevIcon();
		lcd.locate(8,0);
	}

	utils.nextIcon();
	lcd.printf("  Mode");
}
