//controls and manages "pages", and all other lcd content
//TODO: maybe add a special title/connection page?

#include "Pager.h"

extern Serial pc;
extern SPI spi_lcd;

//default constructor, also initializes Display and TextLCD

Pager::Pager(io& ioIn, XS3868& btIn) : bt(btIn), pMusic(lcd, inout, disp, bt), searchText(lcd, "Searching", 6, 1, 8, 200),
connectingText(lcd, "Connecting", 6, 1, 8, 200), connectedText(lcd, "Connected", 6, 1, 8, 200),
disp(lcd), inout(ioIn), lcd(&spi_lcd, D6, D7, TextLCD::LCD16x2, NC, TextLCD::ST7032_3V3) {

	lcd.setContrast(48);
	lcd.setOrient(TextLCD::Top);
	lcd.setInvert(true);

	activePage = 0;					//default page
	disp.init();														//get the lcd ready for use
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
	//connectedText.scroll();
	lcd.cls();
	lcd.locate(0, 0);
	disp.centerText("Connection");
	lcd.locate(0, 1);
	disp.centerText("Successful");
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
