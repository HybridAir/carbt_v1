//used to scroll a section of text in a single row, what more could you want
//text scrolling method is continuous, meaning it doesn't reset at the end, it just loops
//TODO: add a short wait for the first time around

#include "ScrollText.h"

//default constructor, sets up everything the object needs to know
//needs the lcd object, the text to scroll, starting position, length of the display to scroll, speed in ms, and direction
ScrollText::ScrollText(TextLCD_SPI_N& lcdin, string textIn, int col, int row, int length, int speed) : lcd(lcdin) {
	this->textIn = textIn;
	this->col = col;
	this->row = row;
	this->length = length;
	this->speed = speed;
	textLength = textIn.length();
	curPos = 0;																		//used internally, sets the current text scroll position to the beginning

	if(textLength <= length) {														//if the text length is <= the amount of space we get on the LCD
		scrolling = false;															//don't need to scroll anything
	}
	else {
		scrolling = true;															//scrolling is necessary
		this->textIn = textIn + " - ";												//else, add a "space" at the end of the text, to make the loop pretty
		textLength = this->textIn.length();											//recalculate the new length
	}
	firstRun = true;																//this is the first time the object is being ran
	firstWaiting = true;
	ready = false;																	//ready to start scrolling the text
}

//used to actually move the text, must be called continuously to ensure good fps (depending on speed)
void ScrollText::scroll() {
	if(scrolling) {
		if(firstRun) {					//if this is being ran for the first time
			firstWait.start();			//start the firstwait timer
			firstRun = false;			//no longer the first time ran
			lcd.locate(col, row);														//get the lcd in the correct position
			string outString = textIn.substr(curPos, length);   							//cut a out a string starting from the current position, to the usable length
			lcd.printf("%s", outString.c_str());										//print the string to the lcd
		}
		else if(firstWait.read_ms() >= FIRSTDELAY && firstWaiting) {			//wait until the firstwait timer is done
			//used to let the user read the beginning of the text before it starts moving
			ready = true;					//ready to scroll it now
			firstWaiting = false;
			firstWait.stop();
			firstWait.reset();
		}

		if(ready) {														//if it's time to scroll the text one space
			ready = false;

			lcd.locate(col, row);														//get the lcd in the correct position
			string outString;															//get an output string ready

			if(curPos >= textLength) {													//if we are at the end of the text
				curPos = 0;																//put the position back to the beginning; 0
			}

			if(((curPos - 1) >= (textLength - length)) && curPos != 0) {				//if the last part of text is filling up the usable display area, and we're not at the beginning
				//append the end of the text to the beginning, so it "loops" continuously
				string end = textIn.substr(curPos, textLength);							//cut out a string from the current position, to the end of the string
				string start = textIn.substr(0, (length - (textLength - curPos)));		//cut out a string from its beginning, to the amount of empty spaces left in the display area
				outString = end + start;												//combine the two strings strings into one big super string
			}
			else {
				outString = textIn.substr(curPos, length);   							//cut a out a string starting from the current position, to the usable length
			}

			lcd.printf("%s", outString.c_str());										//print the string to the lcd
			curPos++;																	//increment the position
			time.start();																//restart the timer
		}
		else if(time.read_ms() >= speed && !firstWaiting) {												//else, wait until we're ready
			ready = true;
			time.stop();
			time.reset();
		}
	}
	else {							//if not scrolling
		if(firstRun) {					//only do this once
			lcd.locate(col, row);														//get the lcd in the correct position
			lcd.printf("%s", textIn.c_str());										//just print the string to the lcd unmodified
			firstRun = false;
		}
	}

}
