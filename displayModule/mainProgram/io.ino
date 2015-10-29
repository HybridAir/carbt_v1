#define LED_PIN				OCR1B
#define BTN0				PB0
#define BTN1				PB1
#define BTN2				PA7
#define BTN3				PA3

#define F_CPU 8000000UL


void ioInit() {
	//DDRA |= 0x77;		//pa3 and pa7 are inputs, everything else is output
	DDRA &= ~((1<<BTN2)|(1<<BTN3));		//set PA3 and PA7 as inputs
	
	//DDRB |= 0xFC;		//pb0 and pb1 are inputs, everything else are outputs
	DDRB &= ~((1<<PB0)|(1<<PB1));		//set PB0 and PB1 as inputs
	
	//TCCR0A = (1<<COM0A1)|(1<<WGM00);  //pwm for pb2 oc0a, just an extra pwm output if needed later
	TCCR1A = (1<<COM1B1)|(1<<WGM10);  	//pwm for pa5 oc1b, the LCD backlight LED
	
	//set default led brightness and stuff here
	LED_PIN = 10;
}


void writeLED(uint8_t brightness) {
	LED_PIN = brightness;
}


//checks each button for any updates, run this very often
void updateButtons() {
	//uint8_t newBtn = (PINB & 0x03) | (PINA & 0x88);
	
	uint8_t newBtn = (PINB & ((1<<PB0)|(1<<PB1))) | (PINA & ((1<<BTN2)|(1<<BTN3)));
	
	if(newBtn > 0) {
		btnOut = newBtn;
	}
	//btnOut = (PINB & 0x3) | ((PINA & 0x3) << 6);		//get the current values of pb0-1 and pa0-1, then OR them
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