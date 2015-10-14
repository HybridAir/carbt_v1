//watch me as I try to figure out how the hell this thing is going to work
//TODO: add a continuous bt monitor to check if the phone disconnects

#include <mbed.h>
#include "Pager.h"
#include "TextLCD.h"
#include "io.h"
#include "XS3868.h"
#include "BufferedSerial.h"
#include "QN8027.h"											//temporarily disabled


//device objects
Serial pc(USBTX, USBRX); 									//usb serial
BufferedSerial  sbt(PA_11, PA_12);									//xs3868 serial
I2C i2c(D14, D15);											//i2c for fm tx
SPI spi(PC_12, PC_11, PC_10); 								//spi for lcd
TextLCD_SPI_N lcd(&spi, D6, D7, TextLCD::LCD16x2, NC, TextLCD::ST7032_3V3);
XS3868 bt;
QN8027 fm(CRYS_DEFAULT, CRYS24, false, false);				//temporarily disabled


//general objects
io inout;
//Pager pager(lcd, inout, bt);					//temp disable for fm test


//general function prototypes
//void connect();
//void monitorCon();
Timer monitorTimer;
int monitorStatus = 3;


int main() {

	inout.init();					//get io going, can probably move this maybe
	//connect();						//attempt connecting to the bt client			//temp disable for fm test
	//monitorTimer.start();		//temp disable for fm test
	//bt.flushRX();		//temp disable for fm test

	while(1) {
		//monitorCon();

		//if connected, default to music page
		//else, default to stats page

		if(1) {					//temp, replace with a var if menu is active or not
			//handle this in the pager, not out here
			//if menu
			//domenu
			//pager.doPage();		//temp disable for fm test

		}
		else {		//if not menu
			//pager.doPage();		//temp disable for fm test
		}

		//pager.doPage
		//comms monitor

		//if menu button is pressed, switch to menu, and go from there





	}

}


//void connect() {
//	while(bt.connected == false && !bt.bypassBt) {								//while the bt client is not connected and we're not bypassing bt
//
//		int status = bt.connect();												//run the connection process, and get its status
//
//		switch(status) {
//			case 0:																//if searching
//				pager.search();													//tell the user it's searching (only lying half the time)
//				inout.connectionLed(status);									//do the connection led
//				break;
//			case 1:																//if connecting
//				pager.connecting();												//let the user know it's connecting
//				inout.connectionLed(status);									//do the connection led
//				break;
//			case 2:																//if connected
//				break;															//we will deal with this somewhere else
//			case 3:																//if error
//				inout.connectionLed(3);											//turn the connection led off
//				pager.bterror();												//show the error message to the user
//				bt.bypassBt = true;												//bluetooth will not be used
//				break;
//		}
//
//		if(inout.btnPressAll()) {												//if any buttons have been pressed while connecting
//			if(pager.askBypassBt()) {											//ask the user if they want to bypass bt (yes/true)
//				bt.bypassBt = true;												//bypass bt
//				bt.connecting = false;											//let bt know we don't want to connect anymore, so it knows to reset vars
//				inout.connectionLed(3);											//turn connection led off
//			}
//			else {
//				pager.showTitle();												//if the user doesn't want to bypass, show the title again and continue connecting
//			}
//		}
//
//	}
//
//	//at this point we are done connecting
//	if(!bt.bypassBt) {															//if bluetooth will be used
//		pager.connected();														//let the user know it was connected successfully
//		inout.connectionLed(2);													//set the connection led to connected
//		wait(2);																//wait so the user can read the message
//	}
//	//if bluetooth is being bypassed, don't do anything special yet
//}
//
//
////cant use monitorcon and do other stuff
////make a menu to reconnect
////or just always check for II in readstat
//
////OR
////always check if serial is available
////if it is, check for II
////if not II, do nothing else
////that wont work becuase reading it will kill the buffer
//
////monitors the bluetooth connection
//void monitorCon() {
//	if(monitorTimer.read_ms() >= 1000) {
//		monitorStatus = bt.getHFPStatus();
//		monitorTimer.reset();
//	}
//	else {
//		if(monitorStatus != 3 && monitorStatus != 0) {												//device has been disconnected
//			pager.disconnected();
//			wait(2);
//		}
//	}
//		//show connection lost screen title thing
//		//connect()
//}
