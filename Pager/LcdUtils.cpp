//manages lcd utility stuff like custom characters/display, powerup/error? pages, text centering, etc
//TODO

#include "LcdUtils.h"


//default constructor, needs the lcd object
LcdUtils::LcdUtils(TextLCD_SPI_N& lcdIn) : lcd(lcdIn) {

}


void LcdUtils::logoChars() {
	//set the custom logo characters
	lcd.setUDC(0, (char *) top1);
	lcd.setUDC(1, (char *) top2);
	lcd.setUDC(2, (char *) left2);
	lcd.setUDC(3, (char *) left1);
	lcd.setUDC(4, (char *) right2);
	lcd.setUDC(5, (char *) right1);
}


//shows the startup animation and title
void LcdUtils::showStartup() {
	lcd.cls();
	logoChars();
	wait_ms(250);
	lcd.locate(1, 0);
	lcd.putc(0);
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
	wait_ms(1000);
}


//used to show the static title screen
void LcdUtils::showTitle() {
	lcd.cls();
	logoChars();
	lcd.locate(1, 0);
	lcd.putc(0);
	lcd.putc(1);
	lcd.locate(6, 0);
	lcd.printf("carbt_v1");
	lcd.locate(0, 1);
	lcd.putc(3);;
	lcd.putc(2);
	lcd.putc(4);
	lcd.putc(5);
}


//used to clear a specific row
void LcdUtils::clearRow(int row) {
	lcd.locate(0, row);
	for(int x = 0;x < lcd.columns();x++) {						//for each column in the lcd
		lcd.printf(" ");										//write an empty character
	}
}


//used to center a string of text in a single row, text must be less than the display width
void LcdUtils::centerText(string in) {
	char length = in.length();
	char space = (16 - length) / 2;
	for(int x = 0;x < space;x++) {
		lcd.printf(" ");
	}
	lcd.printf("%s", in.c_str());
}


//used to show the custom music control icons, set the lcd cursor to the correct place before calling, they take up 2 spaces each
void LcdUtils::playPauseIcon() {
	musicIcons();
	lcd.putc(1);
	lcd.putc(0);
}

void LcdUtils::prevIcon() {
	musicIcons();
	lcd.putc(2);
	lcd.putc(2);
}

void LcdUtils::nextIcon() {
	musicIcons();
	lcd.putc(1);
	lcd.putc(1);
}

void LcdUtils::musicIcons() {
	lcd.setUDC(0, (char *) pauseChar);
	lcd.setUDC(1, (char *) nextChar);
	lcd.setUDC(2, (char *) prevChar);
}
