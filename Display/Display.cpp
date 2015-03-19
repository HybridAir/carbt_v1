//controls lcd stuff
//TODO: clear row, scroll text

#include "Display.h"

Display::Display() : lcd(D8, D9, D4, D5, D6, D7) {

	//create the custom chars
	lcd.setUDC(0, (char *) top1);
	lcd.setUDC(1, (char *) top2);
	lcd.setUDC(2, (char *) left2);
	lcd.setUDC(3, (char *) left1);
	lcd.setUDC(4, (char *) right2);
	lcd.setUDC(5, (char *) right1);

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

};

void Display::init() {
	//do bluetooth connection
	//and then first page setup in here
	//actually no
	//that stuff can all go in pages.cpp or something
	//only use this for core lcd stuff
}






//void LCDControl::scrollTweet() {                                                //used to scroll the tweet text, must be continuously called
//    if(scroll) {
//        switch(section) {
//            case 0: {                                                           //beginning of tweet section
//                if(printedBegin) {                                              //if we already printed the beginning
//                    unsigned long currentMillis1 = millis();
//                    if(currentMillis1 - previousMillis > opt.getReadTime()) {   //wait for the user read time to elapse
//                        previousMillis = currentMillis1;
//                        section++;                                              //done waiting, allow the program to go to the next section
//                        lcdPos = 0;                                             //reset the lcdPos var, needs to start at 0 after the beginning
//                    }
//                }
//                else {                                                          //did not print the beginning yet
//                    if(currentTweet) {                                          //if we are on the current tweet
//                        printBegin(twt.getTweetBegin());                        //print the beginning
//                    }
//                    else {                                                      //if we are on the previous tweet
//                        printBegin(twt.getPrevBegin());                         //print the previous beginning
//                    }
//                }
//                break;
//            }
//            case 1:  {                                                          //scrolling section
//                if(opt.getScroll()) {
//                    unsigned long currentMillis2 = millis();
//                    if(currentMillis2 - previousMillis > textSpeed) {               //check if it's time to shift the text
//                        previousMillis = currentMillis2;
//                        shiftText();                                                //shift the text by one
//                    }
//                }
//                break;
//            }
//            case 2:   {                                                         //end of tweet section
//                unsigned long currentMillis3 = millis();
//                if(currentMillis3 - previousMillis > opt.getReadTime()) {       //wait for the user read time to elapse
//                    previousMillis = currentMillis3;
//                    section = 0;                                                //done waiting, go back to section 0
//                    printedBegin = false;
//                }
//                break;
//            }
//        }
//    }
//}
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
