//used to scroll a section of text in a single row, what more could you want
//text scrolling method is continuous, meaning it doesn't reset at the end, it just loops

#include "ScrollText.h"


//default constructor, sets up everything the object needs to know
//needs the lcd object, the text to scroll, starting position, length of the display to scroll, speed in ms, and direction
ScrollText::ScrollText(TextLCD& lcdin, string textIn, int col, int row, int length, int speed) : lcd(lcdin) {
	this->textIn = textIn;
	this->col = col;
	this->row = row;
	this->length = length;
	this->speed = speed;
	textLength = textIn.length();
	curPos = 0;													//used internally, sets the current text scroll position to the beginning
	ready = true;												//ready to start scrolling the text

	if(textLength <= length) {					//if the text length is <= the amount of space we get on the LCD
		//don't need to scroll anything
	}
	else {
		textIn = textIn + " - ";				//else, add a "space" at the end of the text, to make the loop pretty
		textLength = textIn.length();			//recalculate the new length
	}
}

//used to actually move the text, must be called continuously to ensure good fps (depending on speed)
void ScrollText::scroll() {
	if(ready) {																		//if it's time to scroll the text one space
		lcd.locate(col, row);														//get the lcd in the correct position
		string outString;															//get an output string ready

		if(curPos > textLength) {													//if we are at the end of the text
			curPos = 0;																//put the position back to the beginning; 0
		}

		if(((curPos - 1) >= (textLength - length)) && curPos != 0) {				//if the last part of text is filling up the usable display area, and we're not at the beginning
			//append the end of the text to the beginning, so it "loops" continuously
			string end = textIn.substr(curPos, textLength);							//cut out a string from the current position, to the end of the string
			string start = textIn.substr(0, (length - (textLength - curPos)));		//cut out a string from its beginning, to the amount of empty spaces left in the display area
			string outString = end + start;											//combine the two strings strings into one big super string
		}
		else {
			string outString = textIn.substr(curPos, length);   			//cut a out a string starting from the current position, to the usable length
		}


		lcd.printf("%s", outString.c_str());				//is that seriously fucking it
		//I love it
	}
	else if(time.read_ms() >= speed) {					//else, wait until we're ready
			ready = true;
			time.stop();
			time.reset();
		}
	}


		//for(int x = 0;textlength;x++)
			//get the current part of the text into a var
			//increase the var by 1
			//create a substring of textin, starting at the new var, and ending at the length
			//print it at the connect position
			//hopefully it works
		//done, reset and start the timer

	lcd.cls();
		lcd.locate(1, 0);
		lcd.printf("%s", textIn.c_str());


}

//
//void LCDControl::shiftText() {                                                  //used to shift the tweet text by one column
//    if(currentTweet) {                                                          //if we are on the current tweet
//        twtLength = twt.getTweetLength();                                       //save the tweet length
//    }
//    else {                                                                      //if we are on the previous tweet
//        twtLength = twt.getPrevLength();                                        //save the previous tweet length
//    }
//    if (lcdPos <= (twtLength - LCDWIDTH)) {
//        //(subtracted LCDWIDTH since we want the ending to use all of LCDWIDTH)
//        if(currentTweet) {                                                      //get the current tweet
//            subTweet = twt.getTweet();
//        }
//        else {                                                                  //or get the previous tweet
//            subTweet = twt.getPrevTweet();
//        }
//        subTweet = subTweet.substring(lcdPos, (lcdPos + LCDWIDTH));             //create a substring from the current position to LCDWIDTH chars ahead
//        lcdc.setCursor(0, 1);                                                   //make sure we print on the bottom row
//        lcdc.print(subTweet);                                                   //printed the shifted substring
//        lcdPos++;                                                               //increase lcdPos by one
//    }
//    if(lcdPos == ((twtLength - LCDWIDTH)+1)) {                                  //check if we are at the end of the text to be shifted
//        section++;                                                              //we are done here, go to the next section
//    }
//}
//
//void LCDControl::setSpeed(int in) {                                             //used to set the text shifting speed
//    textSpeed = in;
//}
