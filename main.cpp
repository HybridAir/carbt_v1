//watch me as I try to figure out how the hell this thing is going to work
//TODO: add a continuous bt monitor to check if the phone disconnects

#include <mbed.h>
#include "Pager.h"
#include "TextLCD.h"
#include "io.h"
#include "XS3868.h"
//#include "QN8027.h"											//temporarily disabled


//device objects
Serial pc(USBTX, USBRX); 									//usb serial
Serial sbt(PA_11, PA_12);									//xs3868 serial
I2C i2c(D14, D15);											//i2c for fm tx
SPI spi(PC_12, PC_11, PC_10); 								//spi for lcd
TextLCD_SPI_N lcd(&spi, D6, D7, TextLCD::LCD16x2, NC, TextLCD::ST7032_3V3);
XS3868 bt;
//QN8027 fm(CRYS_DEFAULT, CRYS24, false, false);				//temporarily disabled


//general objects
io inout;
Pager pager(lcd, inout, bt);
//probably comms or something


int main() {

	inout.init();					//get io going


	while(bt.connected == false && !bt.bypassBt) {								//while the bt client is not connected and we're not bypassing bt
		bt.connect();															//run this continuously to attempt to make a connection
		if(inout.btnPressAll()) {												//if any buttons have been pressed
			if(pager.askBypassBt()) {											//ask the user if they don't want to use bluetooth
				//if the user said they don't want to (yes/true)
				bt.bypassBt = true;												//bypass bt
			}
			else {
				pager.showTitle();												//if the user said yes, show the title again and continue reconnecting
			}
		}
		else {
			wait_ms(5);
			char status = bt.getConStatus();									//get the connection status
			if(status == 1) {													//if the XS3868 is searching for the client
				pager.search();													//let the user know it's searching
				inout.connectionLed(status);									//do the connection led
			}
			else if(status == 2) {												//if the XS3868 is connecting to the client
				pager.connecting();												//let the user know it's connecting
				inout.connectionLed(status);									//do the connection led
			}
			else if(status == 4) {												//if there is a problem with the XS3868
				pager.bterror();												//show the error message to the user
				inout.connectionLed(0);											//turn the connection led off
				bt.bypassBt = true;												//bluetooth will not be used
			}
		}
	}
	if(!bt.bypassBt) {
		//if we got here and the bt is not being bypassed
		while(1) {
			pager.bterror();
		}
		pager.connected();														//let the user know it was connected successfully
		//pager connected doesn't work????
		inout.connectionLed(3);													//do the connection led
	}

	while(1) {
		//if connected, default to music page
		//else, default to stats page

		if(1) {					//temp, replace with a var if menu is active or not
			//if menu
			//domenu
			pager.doPage();

		}
		else {		//if not menu
			pager.doPage();
		}

		//pager.doPage
		//comms monitor

		//if menu button is pressed, switch to menu, and go from there





	}

}
