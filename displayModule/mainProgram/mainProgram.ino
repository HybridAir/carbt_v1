#include "TinySerial.h"
#include <TinyWireM.h> 
#include <ST7032.h>

#define F_CPU        8000000L

#define OCR4800		 52	// div 64
#define OCR9600		 26	// div 64
#define OCR19200	104	// div 8
#define OCR38400	 52	// div 8
#define OCR115200	138	// div 1


const byte top1[] = {0x1, 0x1, 0x3, 0x3, 0x7, 0x7, 0x3, 0x1};
const byte top2[] = {0x10, 0x10, 0x18, 0x18, 0x1c, 0x1c, 0x18, 0x10};
const byte left2[] = {0x18, 0x1c, 0x1e, 0x1e, 0x1e, 0x18, 0x0, 0x0};
const byte left1[] = {0x0, 0x0, 0x1, 0x3, 0x7, 0xf, 0x1e, 0x0};
const byte right2[] = {0x3, 0x7, 0xf, 0xf, 0xf, 0x3, 0x0, 0x0};
const byte right1[] = {0x0, 0x0, 0x10, 0x18, 0x1c, 0x1e, 0xf, 0x0};

const char row0[] = "carbt_v1";
const char row1[] = "starting";


volatile char *inbuf[32];
// outgoing buffer
volatile char *outbuf[32];

ST7032 lcd;


uint8_t btnOut = 0x00;
//byte prevBtn = 0;
bool lcdWrite = false;
	
 
void setup() {

	ioInit();
	commsInit();
	deviceReady();
	
}
 
void loop() {
	checkCommand();
	checkData();
	updateButtons();
	updateLcd();
}


//displays a little animation and info on the LCD, usually when the display module is powered on and ready
void deviceReady() {
	lcd.begin(16, 2);
	writeLED(10);
	logoChars();
	
	lcd.setContrast(255);
	delay(250);
	lcd.setContrast(100);
	delay(250);						//default contrast
	
	lcd.setCursor(1, 0);
	lcd.write(0);
	lcd.write(1);
	writeBuzzer(1);
	delay(125);
	
	lcd.setCursor(0, 1);
	lcd.write(3);
	lcd.write(2);
	writeBuzzer(2);
	delay(125);
	
	lcd.write(4);
	lcd.write(5);
	writeBuzzer(3);
	delay(125);
	writeBuzzer(0);
	
	lcd.setCursor(6, 0);
	for(char i = 0;i < 8; i++) {
		lcd.write(row0[i]);
	}
	
	lcd.setCursor(6, 1);
	for(char i = 0;i < 8; i++) {
		lcd.write(row1[i]);
	}
}


void logoChars() {
	//set the custom logo characters
	lcd.createChar(0, (byte *) top1);
	lcd.createChar(1, (byte *) top2);
	lcd.createChar(2, (byte *) left2);
	lcd.createChar(3, (byte *) left1);
	lcd.createChar(4, (byte *) right2);
	lcd.createChar(5, (byte *) right1);
}