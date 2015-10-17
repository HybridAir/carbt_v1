//handles all communications stuff

//communications registers
#define BTN_READ_REG		0x01
#define LDR_READ_REG 		0x02
//#define TEMP_READ_REG 		0x03
#define LED_WRITE_REG 		0x04
#define CONTRAST_WRITE_REG	0x05
#define CURSOR_WRITE_REG 	0x06
#define START_LCD_REG		0x07
#define STOP_LCD_REG		0x08
#define CLEAR_LCD_REG		0x09


bool newWrite = false;				//newWrite flag
uint8_t currentReg = 0x00;			//current selected register
uint8_t currentData = 0x00;			//data that needs to be processed

uint8_t dataWriteFlag = 0x00;


void commsInit() {
	//mySerial.begin(4800);
	//softSerialInit(DDRA,PORTA,PINA,PA0,PA1);

					 
					 

					 
					 
	softSerialBegin(4800);
	//softSerialWrite('0');
	//mySerial.write('0');
	//send handshake I guess
}


//checks for any commands from the pi, run this very often
void checkCommand() {
	if(dataWriteFlag == 0) {								//if we are not currently waiting for data from the pi
		//check if the pi sent any commands
		//if(mySerial.available() > 0) {					//if the pi sent us something
		if(softSerialAvailable() > 0) {
			//byte command = mySerial.read();				//read the data out of the buffer
			byte command = softSerialRead();
			int tempOut;
			int ldrOut;
			switch(command) {								//do something depending on the received command
				case BTN_READ_REG:						//button status request
					//mySerial.write(btnOut);				//send the last pressed button to the pi
					softSerialWrite(btnOut);
					btnOut = 0x00;						//reset the button variable
					break;
				case LDR_READ_REG:						//LDR value request
					//mySerial.write(updateLDR());		//send the current light level
					ldrOut = updateLDR();
					//mySerial.write(lowByte(ldrOut));
					//mySerial.write(highByte(ldrOut));
					softSerialWrite(lowByte(ldrOut));
					softSerialWrite(highByte(ldrOut));
					break;
/* 				case TEMP_READ_REG:
					//mySerial.write(updateTemp());
					tempOut = updateTemp();
					//mySerial.write(lowByte(tempOut));
					//mySerial.write(highByte(tempOut));
					softSerialWrite(lowByte(tempOut));
					softSerialWrite(highByte(tempOut));
					break; */
				case LED_WRITE_REG:
					dataWriteFlag = LED_WRITE_REG;		//we are now waiting for ledwrite data from the pi
					break;
				case CONTRAST_WRITE_REG:
					dataWriteFlag = CONTRAST_WRITE_REG;		//we are now waiting for ledwrite data from the pi
					break;
				case CURSOR_WRITE_REG:
					dataWriteFlag = CURSOR_WRITE_REG;		//we are now waiting for ledwrite data from the pi
					break;	
				case START_LCD_REG:
					dataWriteFlag = START_LCD_REG;		//we are now waiting for ledwrite data from the pi
					lcdWrite = true;
					break;
				case CLEAR_LCD_REG:
					lcd.clear();
					break;
				default:
					break;
			}
		}
	}
}


void checkData() {
	if(dataWriteFlag > 0 && dataWriteFlag < START_LCD_REG) {					//if we are currently waiting for a data byte from the pi
		//if(mySerial.available() > 0) {		//wait until we get it from the pi
		if(softSerialAvailable() > 0) {
			byte data = softSerialRead();
			//byte data = mySerial.read();
			switch (dataWriteFlag) {
				case LED_WRITE_REG:			//if we were waiting for led brightness data
					writeLED(data);			//send the data to the led
					break;
				case CONTRAST_WRITE_REG:
					lcd.setContrast(data);
					break;
				case CURSOR_WRITE_REG:
					lcd.setCursor((data & 0x0F), ((data & 0x80) >> 7));
					//lcd.setCursor(0, 1);
					break;
				default:
					break;
			}
			dataWriteFlag = 0;				//we got the data, turn the flag off now
		}
	}
}


void updateLcd() {
	if(lcdWrite) {
	//while(lcdWrite) {
		if(softSerialAvailable() > 0) {
			byte data = softSerialRead();
			if(data == STOP_LCD_REG) {
				dataWriteFlag = 0;
				lcdWrite = false;
			}
			else {
				lcd.write(data);
			}
		}
	}
}
