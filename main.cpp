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
Pager pager;
io inout;
XS3868 bt;

int main() {

	while(bt.connected == false) {								//while the bluetooth client is not connected
		bt.connect();									//run this continuously to attempt to make a connection
		//monitor buttons
		char status = bt.getConStatus();				//get the connection status
		if(status == 1) {							//if the XS3868 is searching for the client
			pager.search();
		}
		else if(status == 2) {							//if the XS3868 is connecting to the client
			pager.connecting();
		}
		else if(status == 4) {							//if there is a problem with the XS3868
			pager.bterror();
			//lcd show error
			//print out the error to serial, including the response from bluetooth
			//prompt to skip bt I guess
		}

		//if a button was pressed, prompt for skipping bt

	}

	while(1) {
		//pager.connected();
		pager.bterror();
	}
	//while not connected
		//monitor bt
		//if searching
			//searching on lcd
			//connection led fading
		//else if connecting
			//you know
	//then when connectedf
	//do connected thing on lcd

	while(1) {
		//bt.init();			//initialize the device, includes conencting to the client

	}


}
