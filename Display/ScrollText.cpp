//used to scroll a section of text in a single row, what more could you want
//text scrolling method is continuous, meaning it doesn't reset at the end, it just loops

#include "ScrollText.h"

extern Serial pc;


//default constructor, sets up everything the object needs to know
//needs the lcd object, the text to scroll, starting position, length of the display to scroll, speed in ms, and direction
ScrollText::ScrollText(TextLCD& lcdin, string textIn, int col, int row, int length, int speed) : lcd(lcdin) {
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
	ready = true;																	//ready to start scrolling the text
}

//used to actually move the text, must be called continuously to ensure good fps (depending on speed)
void ScrollText::scroll() {
	if(ready && scrolling) {														//if it's time to scroll the text one space
		ready = false;

		lcd.locate(col, row);														//get the lcd in the correct position
		string outString;															//get an output string ready

		if(curPos >= textLength) {													//if we are at the end of the text
			//pc.printf("got1");
			curPos = 0;																//put the position back to the beginning; 0
		}

		if(((curPos - 1) >= (textLength - length)) && curPos != 0) {				//if the last part of text is filling up the usable display area, and we're not at the beginning
			pc.printf("x");
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
	else if(time.read_ms() >= speed) {												//else, wait until we're ready
		ready = true;
		time.stop();
		time.reset();
	}

}
