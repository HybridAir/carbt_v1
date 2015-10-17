
//#define VOLTAGE				3.4				//input voltage

#define LDR_PIN 			A3				//light dependant resistor pin
#define TEMP_PIN 			A2				//tmp36 pin
#define LED_PIN				5
#define F_CPU 8000000UL

//buttons are on attiny84 pins 10, 9, 8, 7
//or pb0, pb1, pb2, pa7

void ioInit() {
	//DDRB |= 0x03;		//set pb0 and pb1 as inputs
	//DDRA |= 0x03;		//set pa0 and pa1 as inputs
	DDRA |= 0x08;
	//DDRA |= 0x80;
	DDRB |= 0x07;
	
	//set default led brightness and stuff
}


void writeLED(uint8_t brightness) {
	analogWrite(LED_PIN, brightness);
}


//checks each button for any updates, run this very often
void updateButtons() {
	//uint8_t newBtn = (PINB & 0x07) | (PINA & 0x80);
	uint8_t newBtn = (PINB & 0x07) | (PINA & 0x08);
	
	if(newBtn > 0) {
		btnOut = newBtn;
	}
	//btnOut = (PINB & 0x3) | ((PINA & 0x3) << 6);		//get the current values of pb0-1 and pa0-1, then AND them
	//you will get a byte like xx0000xx
	//first two x's are the pina values, then pinb	
	//lcd.setCursor(0,0);
	//lcd.print(btnOut);
	
	//need to check if the last button was the same as this new one
	//since this new method auto resets itself by nature
	//basically if
	//the new value is 0
	//dont keep it and use the last value that was above 0 is possible
	//if reading is > 0x
	//then set it to the var
	//the var needs to be 0 by default and after comms
}


//updates the ldr, and returns its reading
/* int updateLDR() {
	//analogRead(LDR_PIN);
	//uint8_t ldrRead = map(analogRead(LDR_PIN), 0, 1023, 0, 255);
	//return ldrRead;
	return analogRead(LDR_PIN);
} */

//updates the tmp36, and returns its reading in Celsius
/* int updateTemp() {
	//analogRead(TEMP_PIN);
/* 	float tempOut = analogRead(TEMP_PIN);
	tempOut = tempOut * VOLTAGE;
	tempOut = tempOut /= 1024.0; 
	tempOut = (tempOut - 0.5) * 100;
	//return (uint8_t)tempOut;
	return analogRead(TEMP_PIN);
} */