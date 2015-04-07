//used for controlling the OVC3860 based XS3868 board
//TODO: song control, pairing, status, MAKE SENDCMD ALWAYS RETURN A STATUS STRING (all commands return something)

#include "XS3868.h"

extern BufferedSerial sbt;					//serial comms for the XS3868
extern Serial pc;					//temp debug

XS3868::XS3868() {
	pc.printf("\n\rcarbt_v2--------------------------------------------\n\r");
	connected = false;				//assume not connected yet
	connecting = false;				//not connecting either
	bypassBt = false;				//not bypassing bt yet
	connecting = false;
	readLength = 0;

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


//used to clear out the RX hardware buffer
void XS3868::flushRX() {
	while (sbt.readable()) {
		sbt.getc();
	}
}


//used to read in a status response from the device over serial
//returns true if it got something, needs a long enough array to fill
bool XS3868::readStat(char *data) {
	while(sbt.readable()) {									//while the device put something in the rx buffer
		char in = sbt.getc();								//get a char out of the rx buffer

		if(in == '\r') {									//if we got a carriage return
			//ignore it
		}
		else if(in == '\n') {								//if we got a newline
			if(readLength > 0) {							//and if we have some data already, consider it the end of the transfer
				pc.printf("Stat got [%s]\r\n", data);		//temp debug
				readLength = 0;								//reset this for new reads
				flushRX();									//purge the rx buffer of any remaining data
				return true;								//all done
			}
			//else, ignore it
		}
		else {												//else, we're probably getting useful data
			data[readLength] = in;							//put the char into the current data array element
			pc.printf("charin [%c]\r\n", data[readLength]);	//temp debug
			readLength++;									//increment the readLength
		}
	}
	return false;											//if we got here, there is no data yet
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
	if(!connecting) {							//need to reset some variables
		disconnecting = false;
		pairing = false;
		disconnect = false;
		gettingStatus = true;
		connecting = true;
		connectTimer.reset();
		connectTimer.start();
		flushRX();
		response = false;
		checking = false;
	}



	if(gettingStatus) {										//first part of connecting is getting the device's current status
		if(connectTimer.read_ms() >= 1000) {				//try to get the device status every 100 ms if we don't have it yet
			flushRX();										//get any garbage out of the buffer just in case
			sendCmd(BT_STATUS);								//send the status request command
			connectTimer.reset();							//get the 100ms timer reference going
		}
		else {												//if we are waiting
			char stat[3];
			if(readStat(stat)) {							//keep checking the rx buffer for a response
				char status = parseHFPStatus(stat);			//parse the response if we got one
				switch(status) {
					case '1':								//if the device is ready to pair
					case '2':								//or if it's already connecting to the client for some strange reason
						pc.printf("ready to pair\r\n");
						pairing = true;						//continue onto the pairing part
						break;
					case '3':								//if the device has already been paired/connected
						pc.printf("already connected\r\n");
						disconnecting = true;				//disconnect the client to freshen things up, and go on from there
				}
				gettingStatus = false;						//either way we got a status, we are done here
				connectTimer.reset();						//reset the timer for the next part
			}
		}
	}
	else if(disconnecting) {								//disconnecting is optional
		if(connectTimer.read_ms() >= 100) {					//try to get the client to disconnect every 100 ms
			sendCmd(BT_DISCONNECTAV);						//try to disconnect the AV source first
			sendCmd(BT_DISCONNECT);							//and then the client
			connectTimer.reset();							//reset the 100 ms timer
		}
		else {												//if we are waiting
			char stat[3];
			if(readStat(stat)) {							//if we got a response
				if(stat[0] == 'I' && stat[1] == 'A') {		//if we got the disconnected response
					pc.printf("Disconnected successfully");	//temp debug
					connectTimer.reset();
					disconnecting = false;					//done disconnecting
					pairing = true;							//continue onto pairing
				}
				else {
					//idk we got something weird
				}
			}
		}
	}
	else if(pairing) {										//next part of the process is pairing/connecting with the client
		if(t.read_ms() >= 100) {							//try to pair every 100ms
			sendCmd(BT_CONNECT);							//send the pair command
			connectTimer.reset();
		}
		else {

		}

		if(sbt.readable()) {
									            //pc.putc(sbt.getc());
			char str[4];


			//sbt.scanf("%s", str);
			        pc.printf("I got [%s]\r\n", str);
			        if(str[0] == 'I') {
			        	if(str[1] == 'I') {
			        		pc.printf("Disconnect\r\n");
			        	}
			        	else if(str[1] == 'V') {
			        		pc.printf("Connected\r\n");
			        	}
			        }

			        while (sbt.readable()) {
			        	sbt.getc();			//clears out the last character in the buffer that scanf ignores
			        }

	}



//		if(t.read_ms() >= 5000) {				//send the connect command once every second while trying to pair
//			//sendCmd(BT_CONNECT);
//			sendCmd(BT_STATUS);
//			t.reset();
//		}
	}
	//t.stop();




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


//parses the HFP status response (MG#) from the device, returns the status as a number char, and needs the char array to parse
char XS3868::parseHFPStatus(char* stat) {
	if(stat[0] == 'M' && stat[1] == 'G') {					//make sure it's a HFP status response
		if(stat[2] == '1') {								//ready
			return '1';
		}
		else if(stat[2] == '2') {							//connecting
			return '2';
		}
		else if (stat[2] > '2') {							//connected (and other, but still connected)
			return '3';
		}
	}
	else if(stat[0] == 'I' && stat[1] == 'V') {				//if it's a state response for some reason (shouldn't ever get here)
		return '3';
	}
	else {													//if it's something else
		pc.printf("BT_ERROR:[%s]\n\r", stat);					//we got a bad response, print out the error over serial
		return '4';											//bad response code
	}
}


//parses the State status response (I--) from the device, returns the status as a number char, and needs the char array to parse
char XS3868::parseState(char* stat) {
	if(stat[0] == 'I') {									//ensure it's a state response
		switch(stat[1]) {
		case 'I':											//if the device is in pairing mode
			return '1';										//device is ready to pair, and searching for the client
			break;
		case 'V':											//if the device has connected successfully
			return '3';										//yeah
		default:											//if we get anything else (we shouldn't I think)
			return '4';										//throw a bterror
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


//void XS3868::disconnect() {
//	sendCmd(BT_RESET);
//}


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
