//used for controlling the OVC3860 based XS3868 board
//TODO: song control, pairing, status

#include "XS3868.h"

Serial sbt(PA_11, PA_12); // tx, rx
Serial pc(USBTX, USBRX); // tx, rx

XS3868::XS3868() {
	//still not sure why any code put in here doesn't do anything

	//reset?
	//check if connected	 DONT HAVE TO DO THIS NEVERMIND
	//if connected, continue as usual
	//else
	//conect then continue
}


//used to send a command to the device over serial
void XS3868::sendCmd(string command) {
	pc.printf("%s%s%s", BT_PREFIX.c_str(), command.c_str(), BT_SUFFIX.c_str());		//temp debug
	sbt.printf("%s%s%s", BT_PREFIX.c_str(), command.c_str(), BT_SUFFIX.c_str());
	//pc.printf("AT#MA\r\n");
	//sbt.printf("AT#MA\r\n");
}


//used to read in a status response from the device over serial
int XS3868::readStat(char *data) {
	if(sbt.readable()) {						//if there is new data coming in through serial
		 char buffer[3];
	     sbt.scanf("%s", &buffer);
	            //pc.printf("%c", buffer[0]);
	            //pc.printf("%c", buffer[1]);
	            //pc.printf("%c", buffer[2]);
	     //data = buffer;
	            strncpy(data, buffer, sizeof(data));
	     return 1;								//new data
	}
	else {
		return 0;								//no new data
	}
	//maybe have it take an output var for returning the payload
	//and return it int to let its parent know if it got anything
	//useful for having it in while(nothing)'s

}


//sets up the device for operation
void XS3868::init() {
	wait(3);					//wait for the device to start if it was previously off
	sbt.baud(115200);
	//sendCmd(BT_CONNECT);
	//sendCmd(BT_CONNECTAV);
	connect();
	//wait until we get a connect response
	wait(5);		//temp
	sendCmd(BT_PLAYPAUSE);
	wait(5);
	sendCmd(BT_DISCONNECTAV);
	sendCmd(BT_DISCONNECT);

}

void XS3868::connect() {

	//if it responds with IV, connection was successful
	sendCmd(BT_QA2DP);
//	 char buffer[20];
//    while(1) {
//        if (sbt.readable()) {
//        	sbt.scanf("%s", &buffer);
//            printf("%c", buffer[0]);
//            printf("%c", buffer[1]);
//            printf("%c", buffer[2]);
//        }
//      }
	char dataIn[3];
	bool waiting = true;
	while(waiting) {
		if(readStat(dataIn) != 0) {
			waiting = false;
			pc.printf("got a thing:");
			pc.printf("%s", dataIn);
		}
	}


		sendCmd(BT_CONNECT);			//connect to the last known paired device
	sendCmd(BT_CONNECTAV);				//connect to the device's audio source

}

void XS3868::disconnect() {
	sendCmd(BT_DISCONNECT);
}
