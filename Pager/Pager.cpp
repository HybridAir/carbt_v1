//controls and manages "pages", and all other lcd content

#include "Pager.h"

extern Serial pc;
//ScrollText search(lcd, "Searching for phone", 6, 1, 8, 200);

Pager::Pager(io& ioIn) : searchText(lcd, "Searching", 6, 1, 8, 200),
connectingText(lcd, "Connecting", 6, 1, 8, 200), connectedText(lcd, "Connected", 6, 1, 8, 200),
disp(lcd), lcd(D8, D9, D4, D5, D6, D7), inout(ioIn) {				//default constructor, also initializes Display and TextLCD
	wait_ms(250);
	disp.init();														//get the lcd ready for use
	init();

}


//shows the boot up animation and title
void Pager::init() {
	lcd.cls();
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
}


//used to show the static title screen
void Pager::showTitle() {
	lcd.cls();
	lcd.locate(1, 0);
	lcd.putc(0);
	lcd.putc(1);
	lcd.locate(0, 1);
	lcd.putc(3);;
	lcd.putc(2);
	lcd.putc(4);
	lcd.putc(5);
	lcd.locate(6, 0);
	lcd.printf("carbt_v1");
}

void Pager::search() {
	searchText.scroll();
}

void Pager::connecting() {
	connectingText.scroll();
}

void Pager::connected() {
	connectedText.scroll();
}


//used to tell the user that there was a bluetooth error, and asks them to continue
void Pager::bterror() {
	ScrollText bterrorText(lcd, "Failed to connect: BT_ERROR. Select Ok to continue without using Bluetooth.", 0, 0, 16, 200);
	disp.clearRow(1);										//clear the bottom row
	lcd.locate(7,1);
	lcd.printf("Ok");										//print "ok" at the center of the lcd
	Timer indicatorTime;
	indicatorTime.start();									//start a timer for blinking the indicator character
	bool blink = true;
	while(!inout.pressSelect()) {							//while the select button has not been pressed yet
		if(indicatorTime.read_ms() >= 500) {				//blink the indicator character every 500 ms
			//becuase it looks cool I don't know
			if(blink) {
				blink = false;
			}
			else {
				blink = true;
			}
			indicatorTime.reset();
		}

		lcd.locate(6,1);
		if(blink) {											//print the indicator or an empty space depending on what it's time to print
			lcd.putc(0x7E);									//0x7E is the right arrow character
		}
		else {
			lcd.printf(" ");
		}
	}
	bterrorText.scroll();
}


//used to ask the user if they want to skip connecting to the bluetooth device (usually becuase we can't find it, or there's another problem)
bool Pager::askBypassBt() {
	ScrollText bypass(lcd, "Do you want to skip connecting to bluetooth?", 0, 0, 16, 200);
	Prompt askBypass(lcd, disp, inout, "Yes", "No", 1);
	//bool asking = true;
	while(1) {
		bypass.scroll();
		char answer = askBypass.ask();
		//pc.printf("prompt:%d\r\n", askBypass.ask());
		if(answer == 1) {							//user selected yes
			return true;
		}
		else if(answer == 2) {						//user selected no
			return false;
			//asking = false;
		}
	}
}
