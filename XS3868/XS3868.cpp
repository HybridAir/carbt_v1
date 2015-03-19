//used for controlling the OVC3860 based XS3868 board
//TODO: song control, pairing, status

#include "XS3868.h"

Serial sbt(PA_11, PA_12); // tx, rx
Serial pc(USBTX, USBRX); // tx, rx

XS3868::XS3868() {

}


//used to send a command to the device over serial, wants the COMMAND string
void XS3868::sendCmd(string command) {
	pc.printf("%s%s%s", BT_PREFIX.c_str(), command.c_str(), BT_SUFFIX.c_str());		//temp debug
	sbt.printf("%s%s%s", BT_PREFIX.c_str(), command.c_str(), BT_SUFFIX.c_str());
	//pc.printf("AT#MA\r\n");
	//sbt.printf("AT#MA\r\n");
}


//used to read in a status response from the device over serial, returns true if it got something, and needs a data array to fill
bool XS3868::readStat(char *data) {
	if(sbt.readable()) {						//if there is new data coming in through serial
		 char buffer[3];						//get the buffer ready
	     sbt.scanf("%s", &buffer);				//get everything in
	     strncpy(data, buffer, sizeof(data));	//get everything out of the buffer and into the data array
	     return true;								//new data
	}
	else {
		return false;								//no new data
	}
}


//prepares the device for operation
void XS3868::init() {
	pc.printf("\n\rcarbt_v2\n\r");
	connected = false;				//assume not connected
	sbt.baud(115200);
	//wait(3);						//wait for the device to power up if it was previously off
	connect();
	sendCmd(BT_PLAYPAUSE);
	wait(5);
	sendCmd(BT_DISCONNECTAV);
	sendCmd(BT_DISCONNECT);

}


//handles connecting to the bluetooth device, call this frequently
void XS3868::connect() {
	//first try to get the status until it's nonzero
	//if the status is 1, proceed with connecting
	//if 3, skip connecting, and continue
	//if 2 or 4, consider it an error and tell the user to back the fuck up
	char status = getConStatus();
	if(status != 0) {				//the device responded
		if(status == 1) {			//the device is ready to connect
			//connect it in here
			//check status after to make sure it did it
		}
		else if(status == 2 || status == 4) {			//if we get a response error
			//back the fuck up
			//the 2 could be useful above actually, going to remove it from here soon
		}
		else if(status == 3) {
			//we;re already connected from before, don't need to reconnect
			//might want to make sure a2dp and avrcp are ready do
			//ill do that later
		}
	}
	//else
		//go do something else while we wait for the device to respond
		//don't forget to keep spamming this when possible if you plan on connecting


//	char dataIn[3];
//	bool waiting = true;
//	while(waiting) {
//		sendCmd(BT_STATUS);					//get the bt connection status
//		if(readStat(dataIn) != 0) {
//			//waiting = false;
//			pc.printf("response:");
//			pc.printf("%s", dataIn);
//
//			if(dataIn[0] == 'M' && dataIn[1] == 'G') {			//make sure it's the correct response
//				pc.printf(":%c", dataIn[2]);
//				if(dataIn[2] == '1') {
//					sendCmd(BT_CONNECT);			//connect to the last known paired device
//					sendCmd(BT_CONNECTAV);				//connect to the device's audio source
//				}
//				else if(dataIn[2] == '3') {
//					waiting = false;
//				}
//			}
//		}
//		wait(1);
//	}


	//	sendCmd(BT_CONNECT);			//connect to the last known paired device
	//sendCmd(BT_CONNECTAV);				//connect to the device's audio source

}


//extracts and returns the State Query status number, use it to get the bluetooth connection status
//call it continuously until you get something not 0
//0: no response, device is possibly offline, keep trying
//1: Ready
//2: Connecting
//3: Connected
//4: bad response, device sent garbage or a response to a different command
char XS3868::getConStatus() {
	char dataIn[3];									//status array
	sendCmd(BT_STATUS);								//send the State Query commands (CY)
	//wait x ms thread safe
	//need to make sure no other commands are send while waiting
	//we will get a bad response if that happens
	if(readStat(dataIn)) {							//if we got a response
		pc.printf("response:");
		pc.printf("%s", dataIn);

		if(dataIn[0] == 'M' && dataIn[1] == 'G') {			//make sure it's the correct response type
			if(dataIn[2] == '1') {
				return 1;
			}
			else if(dataIn[2] == '2') {
				return 2;
			}
			else if (dataIn[2] > '2') {
				return 3;
			}
		}
		else {
			return 4;						//bad response
		}
	}

}

void XS3868::disconnect() {
	sendCmd(BT_DISCONNECT);
}
