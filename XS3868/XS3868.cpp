//used for controlling the OVC3860 based XS3868 board
//TODO: song control, pairing, status, MAKE SENDCMD ALWAYS RETURN A STATUS STRING (all commands return something)

#include "XS3868.h"

extern Serial sbt;					//serial comms for the XS3868
extern Serial pc;					//temp debug

XS3868::XS3868() {
	pc.printf("\n\rcarbt_v2--------------------------------------------\n\r");
	connected = false;				//assume not connected yet
	connecting = false;				//not connecting either
	bypassBt = false;				//not bypassing bt yet
	sbt.baud(115200);
	wait(1);						//give the XS3868 some time to power up
}


//used to send a command to the device over serial, wants the COMMAND string
void XS3868::sendCmd(string command) {
	//send the prefix, command, and suffix over serial
	pc.printf("write attempt\r\n");
	if(sbt.writeable()) {
		pc.printf("sending: %s%s%s\r\n", BT_PREFIX.c_str(), command.c_str(), BT_SUFFIX.c_str());
		sbt.printf("%s%s%s", BT_PREFIX.c_str(), command.c_str(), BT_SUFFIX.c_str());
	}
}


//used to read in a status response from the device over serial, returns true if it got something, and needs a data array to fill
//this needs to be rate limited
bool XS3868::readStat(char *data) {
	pc.printf("Read attempt\r\n");
	wait(1);
	while(sbt.readable()) {						//if there is new data coming in through serial
		wait(1);
		pc.printf("Got something\r\n");
		pc.printf("=%X\r\n", sbt.getc());
	}




		//pc.printf("Got something\r\n");
		 //char buffer[20];						//get the buffer ready

	     ////sbt.scanf("%s", &buffer);				//get everything in
	     //pc.printf("read: %X", buffer[0]);
	     //pc.printf(" %X", buffer[1]);
	     //pc.printf(" %X\r\n", buffer[2]);
	     //strncpy(data, buffer, sizeof(data));	//get everything out of the buffer and into the data array

	     return true;								//new data
	//}
	//else {
	//	pc.printf("Read fail\r\n");
	//	return false;								//no new data
	//}
}


//prepares the device for operation
//temp, probably dont need you
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
//give this access to pager?
void XS3868::connect() {
	//disconnect first always

	pairing = true;

	Timer t;
	t.start();

	while(pairing) {
//		if(newResponse) {					//if we got something from the device
//			//check it
//			//if it's good
//			pairing = false;
//		}

		if(sbt.readable()) {
									            //pc.putc(sbt.getc());
			char str[10];
			sbt.scanf("%s", str);
			        pc.printf("I got [%s]\r\n", str);

			        //while (sbt.readable()) {
			        	sbt.getc();			//clears out the last character in the buffer that scanf ignores
			        //}

			    }



		if(t.read_ms() >= 5000) {				//send the connect command once every second while trying to pair
			sendCmd(BT_CONNECT);
			t.reset();
		}
	}
	t.stop();




//	if(!connected) {										//if the device is not connected to the bluetooth client
//		//char status = getConStatus();						//get the connection status from the device
//		char status = 1;
//		if(status != 0) {									//if the device responded
//			if(status == 1) {								//if the device is ready to connect
//				if(!connecting) {							//if the connection commands have not been sent yet, send them only once
//					while(1) {
//						sendCmd(BT_CONNECT);					//connect to the last known paired device
//
//
//						//char dataIn[3];
//
//						//wait(1);
//															//status array
//												//readStat(dataIn);
//						while(1) {
//							if(sbt.readable()) {
//							            pc.putc(sbt.getc());
//							        }
//						}
//					}
//						//sendCmd(BT_CONNECTAV);					//make sure we get the audio source (?) connected too
//
//						//connecting = true;						//connecting is in progress
//				}
//			}
//			else if(status == 2) {							//if the device is currently connecting
//				//checking for this doesn't exactly matter, but we should probably let the user know
//			}
//			else if(status == 3) {						//device is connected
//				//if we're already connected from before, we don't need to reconnect
//				//might want to make sure a2dp and avrcp are ready do
//				//later
//				//connected = true;
//			}
//			else if(status == 4) {			//if we get a response error
//				//go tell the user to back the fuck up
//			}
//		}
//	}
//	//else
//		//go do something else while we wait for the device to respond
//		//don't forget to keep spamming this when possible if you plan on connecting

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
		//pc.printf("response:");
		//pc.printf("%s\n\r", dataIn);

		if(dataIn[0] == 'M' && dataIn[1] == 'G') {			//make sure it's the correct response type
			if(dataIn[2] == '1') {
				return 1;
			}
			else if(dataIn[2] == '2') {
				return 2;
			}
			else if (dataIn[2] > '2') {
				connected = true;
				return 3;
			}
		}
		else if(dataIn[0] == 'I' && dataIn[1] == 'V') {
			connected = true;
			return 3;
		}
		else {
			pc.printf("BT_ERROR:%s\n\r", dataIn);					//we got a bad response, print out the error over serial
			return 4;												//bad response code
		}
	}
}


//returns whether music is playing or not
bool XS3868::getSongStatus() {
	char dataIn[2];									//status array
	sendCmd(BT_CONNECTAV);								//send the State Query commands (CY)
	pc.printf("send\r\n");
	if(readStat(dataIn)) {							//if we got a response
		//pc.printf("stat:%s\r\n", dataIn);
		if(dataIn[0] == 'M') {			//make sure it's the correct response type
			if(dataIn[1] == 'A') {
				return false;
			}
			else {
				return true;
			}
		}
	}
	pc.printf("done\r\n");

	return false;
}


void XS3868::disconnect() {
	sendCmd(BT_RESET);
}


//toggles the music play/pause status on the bt client, returns the new status (true = playing, false = paused)
bool XS3868::playPause() {

	sendCmd(BT_PLAYPAUSE);
	char dataIn[3];					//status array
	readStat(dataIn);
	if(dataIn[1] == 'A') {
		return false;
	}
	else {
		return true;
	}

}
