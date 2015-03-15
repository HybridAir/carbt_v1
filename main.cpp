//watch me as I try to figure out what the hell this thing is going to do
#include <mbed.h>
#include "TextLCD.h"
#include "io.h"

Serial pc(USBTX, USBRX); // tx, rx
Serial bt(PA_11, PA_12); // tx, rx

DigitalOut myled(LED1);
DigitalIn Prev(D10);
DigitalIn Play(D11);
DigitalIn Next(D12);
DigitalIn Menu(D13);

TextLCD lcd(D8, D9, D4, D5, D6, D7);  // RS, E, DB4, DB5, DB6, DB7

int main() 
{
	lcd.locate(0, 0);
	lcd.printf("Music Control");
	pc.printf("Music Control\n");

	//bt.printf("AT#CZ\n\r");

	bt.baud(115200);
	wait(3);
	bt.printf("AT#CC\r\n");
    while(1) 
    {
    	if(Prev) {
    		lcd.locate(0, 1);
    		lcd.printf("Prev");
    	}
    	else if(Play) {
    	    		lcd.locate(0, 1);
    	    		lcd.printf("Play");
    	    		bt.printf("AT#MA\r\n");
    	    	}
    	else if(Next) {
    	    		lcd.locate(0, 1);
    	    		lcd.printf("Next");
    	    	}
    	else if(Menu) {
    	    		lcd.locate(0, 1);
    	    		lcd.printf("Menu");
    	    	}
    	else {
    		lcd.locate(0, 1);
    		lcd.printf("    ");
    	}
    }
}
