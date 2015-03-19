//used for controlling the OVC3860 based XS3868 board
//TODO: song control, pairing, status

#include "XS3868.h"

extern Serial sbt;
extern Serial pc;

XS3868::XS3868() {
	pc.printf("\n\rcarbt_v2\n\r");
	connected = false;				//assume not connected
	connecting = false;
	sbt.baud(115200);
	wait(5);
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
	connect();
	if(connected) {
		//wait before doing any music commands, the client has to catch up
		wait(2);
	sendCmd(BT_PLAYPAUSE);
	wait(10);
	sendCmd(BT_DISCONNECTAV);
	sendCmd(BT_DISCONNECT);
	//wait(20);
	}

}


//handles connecting to the bluetooth client, call this frequently
//might have to make it return the status later
void XS3868::connect() {
	if(!connected) {										//if the device is not connected to the bluetooth client
		char status = getConStatus();						//get the connection status from the device
		if(status != 0) {									//if the device responded
			if(status == 1) {								//if the device is ready to connect
				if(!connecting) {							//if the connection commands have not been sent yet, send them only once
					sendCmd(BT_CONNECT);					//connect to the last known paired device
					sendCmd(BT_CONNECTAV);					//make sure we get the audio source (?) connected too
					connecting = true;						//connecting is in progress
				}
			}
			else if(status == 2) {							//if the device is currently connecting
				//checking for this doesn't exactly matter
				//but we should probably let the user know
				//later
			}
			else if(status == 3) {						//device is connected
				//if we;re already connected from before, we don't need to reconnect
				//might want to make sure a2dp and avrcp are ready do
				//later
				connected = true;
			}
			else if(status == 4) {			//if we get a response error
				//go tell the user to back the fuck up
			}
		}
	}
	//else
		//go do something else while we wait for the device to respond
		//don't forget to keep spamming this when possible if you plan on connecting

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
		pc.printf("%s\n\r", dataIn);

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
