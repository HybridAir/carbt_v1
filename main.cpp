//watch me as I try to figure out how the hell this thing is going to work

#include <mbed.h>
#include "Pager.h"
#include "io.h"
#include "XS3868.h"
//#include "QN8027.h"											//temporarily disabled


Serial pc(USBTX, USBRX); // tx, rx
Serial sbt(PA_11, PA_12);
I2C i2c(D14, D15);
//QN8027 fm(CRYS_DEFAULT, CRYS24, false, false);				//temporarily disabled
io inout;
Pager pager(inout);
XS3868 bt;

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
			char status = bt.getConStatus();									//get the connection status
			if(status == 1) {													//if the XS3868 is searching for the client
				pager.search();													//let the user know it's searching
				//fade connection led
			}
			else if(status == 2) {												//if the XS3868 is connecting to the client
				pager.connecting();												//let the user know it's connecting
				//fade connection led
			}
			else if(status == 4) {							//if there is a problem with the XS3868
				pager.bterror();							//show the error message
				bt.bypassBt = true;
				//make the user select ok from a prompt
				//print out the error to serial, including the response from bluetooth
			}
		}
	}
	if(!bt.bypassBt) {															//if we got here and the bt is not being bypassed
		pager.connected();														//let the user know it was connected successfully
	}

	while(1) {
		inout.led(inout.btnReadAll());				//temp
	}

}
