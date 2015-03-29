//controls and manages "pages", and all other lcd content
//TODO: maybe add a special title/connection page, move connection stuff to its own page
//PagerUtils?

#include "Pager.h"

extern Serial pc;


//default constructor, wants the lcd, io, and XS3868 objects
Pager::Pager(TextLCD_SPI_N& lcdIn, io& ioIn, XS3868& btIn) : bt(btIn), pMusic(lcd, utils, inout, bt), searchText(lcd, "Searching", 6, 1, 8, 200),
connectingText(lcd, "Connecting", 6, 1, 8, 200), connectedText(lcd, "Connected", 6, 1, 8, 200),
inout(ioIn), utils(lcd), lcd(lcdIn) {

	lcd.setContrast(48);

	activePage = 0;					//default page


	lcd.setUDC(0, (char *) top1);
		lcd.setUDC(1, (char *) top2);
		lcd.setUDC(2, (char *) left2);
		lcd.setUDC(3, (char *) left1);
		lcd.setUDC(4, (char *) right2);
		lcd.setUDC(5, (char *) right1);



	//disp.init();														//get the lcd ready for use
	init();
	newPage = true;
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
	wait_ms(250);
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


//used to display a specific page on the display, must be ran continuously
void Pager::doPage() {
	//get the current page
	//if the current != the old page
	//end the old page and init the new (current) page
	//old = current

	//or
	//get the current page
	//display it
	//the only time the page will change is if the menu was just shown

	switch(activePage) {
		case 0:
			pMusic.showPage(newPage);
		break;
		default:
		break;
	}

	newPage = false;
}


//used to display the page switching menu, must be ran continuously
void Pager::doMenu() {

}







void Pager::search() {
	searchText.scroll();
}

void Pager::connecting() {
	connectingText.scroll();
}

void Pager::connected() {
	//Display disp2(lcd);

	lcd.cls();
	lcd.locate(0, 0);
	utils.centerText("Connection");
	lcd.locate(0, 1);
	utils.centerText("Successful");
}


//used to tell the user that there was a bluetooth error, and asks them to continue
void Pager::bterror() {
	//Display disp3(lcd);
	ScrollText bterrorText(lcd, "Failed to connect: BT_ERROR. Select Ok to continue without using Bluetooth.", 0, 0, 16, 200);
	utils.clearRow(1);										//clear the bottom row
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
		bterrorText.scroll();
	}

}


//used to ask the user if they want to skip connecting to the bluetooth device (usually becuase we can't find it, or there's another problem)
bool Pager::askBypassBt() {
	ScrollText bypass(lcd, "Do you want to skip connecting to bluetooth?", 0, 0, 16, 200);
	Prompt askBypass(lcd, utils, inout, "Yes", "No", 1);
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
