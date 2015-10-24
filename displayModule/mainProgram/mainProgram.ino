#include "TinySerial.h"
//#include <Bounce2.h>
//#include <SoftwareSerial.h>
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


volatile char *inbuf[32];
// outgoing buffer
volatile char *outbuf[32];

ST7032 lcd;


uint8_t btnOut = 0x00;
//byte prevBtn = 0;
bool lcdWrite = false;
	
 
void setup() {
	lcd.begin(16, 2);
	lcd.setContrast(150);
	logoChars();
	lcd.setCursor(1, 0);
	lcd.write(0);
	lcd.write(1);
	lcd.setCursor(0, 1);
	lcd.write(3);;
	lcd.write(2);
	lcd.write(4);
	lcd.write(5);

	ioInit();
	commsInit();
	
	//red led pwm on pb2 oc0a
	//TCCR0A = (1<<COM0A1)|(1<<WGM00);  // mode #1
	//TCCR0B = (2<<CS00);  // div8
	//OCR0A = 0;				//highest freq 3230
	
	//lcd led on pa5 oc1b
	//TCCR1A = (1<<COM1B1)|(1<<WGM10);  // mode #1
	OCR1B = 10;
	
	PORTA |= (1<<PA2);
	delay(125);
	PORTB |= (1<<PB2);
	delay(125);
	PORTA &= ~(1<<PA2);
	delay(125);
	PORTB &= ~(1<<PB2);
	
}
 
void loop() {
	checkCommand();
	checkData();
	updateButtons();
	updateLcd();
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