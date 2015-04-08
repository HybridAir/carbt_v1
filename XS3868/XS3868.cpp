//used for controlling the OVC3860 based XS3868 board
//TODO: song control

#include "XS3868.h"


extern BufferedSerial sbt;					//serial comms for the XS3868
extern Serial pc;							//temp debug


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


//sends a command to the device over serial, wants the COMMAND string
void XS3868::sendCmd(string command) {
	if(sbt.writeable()) {								//if there is room to write something
		pc.printf("sending [%s%s]\r\n\n", BT_PREFIX.c_str(), command.c_str());
		//send the prefix, command, and suffix over serial
		sbt.printf("%s%s%s", BT_PREFIX.c_str(), command.c_str(), BT_SUFFIX.c_str());
	}
}


//clears out the RX buffer
void XS3868::flushRX() {
	while (sbt.readable()) {							//while there is stuff in the buffer
		sbt.getc();										//read it out to infinity
	}
}


//reads in a status response from the device over serial
//returns true if it got something, needs a char array of the correct size to fill
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


//handles connecting to the bluetooth client, call this frequently
//returns the current status; 0: no response/searching, 1: connecting, 2: connected, 3: error
//give this access to pager?
int XS3868::connect() {
	if(!connecting) {												//need to reset some variables
		disconnecting = false;
		pairing = false;
		disconnect = false;
		gettingStatus = true;										//begin with getting the device's current HFP status
		connecting = true;											//we are connecting now
		connectTimer.reset();										//get the command delay timer ready
		connectTimer.start();
		flushRX();													//get any garbage out of the rx buffer
	}

	if(gettingStatus) {												//first part of connecting is getting the device's current status
		if(connectTimer.read_ms() >= 100) {							//try to get the device status every 100 ms if we don't have it yet
			flushRX();												//get any garbage out of the buffer just in case
			sendCmd(BT_STATUS);										//send the status request command
			connectTimer.reset();									//get the 100ms timer reference going
			return 0;												//this is effectively the end, let them know we're searching now
		}
		else {														//if we are waiting
			char stat[4];
			if(readStat(stat)) {									//keep checking the rx buffer for a response
				char status = parseHFPStatus(stat);					//parse the response if we got one

				gettingStatus = false;								//we got a status, we are done after this
				connectTimer.reset();								//reset the timer for the next part
				switch(status) {
					case '1':										//if the device is ready to pair
					case '2':										//or if it's already connecting to the client for some strange reason
						pc.printf("ready to pair\r\n");
						pairing = true;								//continue onto the pairing part
						return 0;									//ready to connect, but still searching
						break;
					case '3':										//if the device has already been paired/connected
						pc.printf("already connected\r\n");
						disconnecting = true;						//disconnect the client to freshen things up, and go on from there
						return 0;									//still "searching"
				}
			}
		}
	}
	else if(disconnecting) {										//disconnecting is optional
		if(connectTimer.read_ms() >= 100) {							//try to get the client to disconnect every 100 ms
			flushRX();												//get any garbage out of the buffer just in case
			sendCmd(BT_DISCONNECT);									//send the client disconnect command to the device
			connectTimer.reset();									//reset the 100 ms timer
			return 0;												//searching
		}
		else {														//if we are waiting
			char stat[4];
			if(readStat(stat)) {									//if we got a response
				if(stat[0] == 'I' && stat[1] == 'A') {				//if we got the disconnected response
					pc.printf("Disconnected successfully\r\n");			//temp debug
					connectTimer.reset();
					disconnecting = false;							//done disconnecting
					pairing = true;									//continue onto pairing
					return 0;										//now we are 'searching'
				}
				else {
					pc.printf("disconnecting BT_ERROR:[%s]\n\r", stat);			//we got something weird, throw a bterror
					return 3;										//we have a problem
				}
			}
		}
	}
	else if(pairing) {												//next part of the process is pairing/connecting with the client
		if(connectTimer.read_ms() >= 500) {									//try to pair every 100ms
			flushRX();												//get any garbage out of the buffer just in case
			sendCmd(BT_CONNECT);									//send the pair command
			connectTimer.reset();
			return 0;												//connecting
		}
		else {
			char stat[4];
			if(readStat(stat)) {									//keep checking the rx buffer for a response
				char status = parseState(stat);						//parse the response if we got one
				switch(status) {
					case '1':										//if the device is in pairing mode and searching for the client
						return 0;									//connecting
						break;										//don't do anything, just wait indefinitely
					case '2':										//if the device successfully paired
						pc.printf("Paired successfully\r\n");
						pairing = false;							//done pairing
						connecting = false;							//no longer connecting
						connected = true;							//and we are finally connected
						connectTimer.stop();
						connectTimer.reset();
						return 2;									//we are connected
						break;
					case '3':										//something weird happened
						pc.printf("pairing got something weird\r\n");
						return 3;									//we have a problem
						break;
				}
			}
			else {
				return 0;											//searching still
			}
		}
	}
}


//parses the HFP status response (MG#) from the device, returns the status as a number char, and needs the char array to parse
//make this return an int
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


//returns the current HFP status, needs to be called frequently if you want a prompt response
//make this return an int
int XS3868::getHFPStatus() {
	char stat[4];
	if(readStat(stat)) {									//if we got a response
		char status = parseHFPStatus(stat);					//parse the response
		switch(status) {
			case '1':										//if the device is ready to pair
				return 1;
				break;
			case '2':										//if the device is connecting
				return 2;
				break;
			case '3':										//if the device is connected
				return 3;
				break;
			case '4':										//if we got a parse error
				return 4;									//let them deal with it
				break;
		}
	}
	else {													//if there is nothing yet
		flushRX();											//get any garbage out of the buffer just in case
		sendCmd(BT_STATUS);									//try to get the HFP status
		return 0;											//no response yet
	}
}


//parses the State status response (I--) from the device, returns the status as a number char, and needs the char array to parse
//make this return an int
char XS3868::parseState(char* stat) {
	if(stat[0] == 'I') {									//ensure it's a state response
		switch(stat[1]) {
		case 'I':											//if the device is in pairing mode
			return '1';										//device is ready to pair, and searching for the client
			break;
		case 'V':											//if the device has connected successfully
			return '2';										//yeah
		default:											//if we get anything else (we shouldn't I think)
			pc.printf("BT_ERROR:[%s]\n\r", stat);			//throw a bterror
			return '3';
		}
	}
}


//returns whether music is playing or not, 0: no response, 1: playing, 2: paused
//call it frequently for a prompt response
int XS3868::getSongStatus() {
	char stat[3];
	if(readStat(stat)) {											//if we got a response
		if(stat[0] == 'M') {										//make sure it's the right response type
			if(stat[1] == 'A') {
				return 1;											//playing
			}
			else if(stat[1] == 'B') {
				return 2;											//pasued
			}
			else {
				pc.printf("getSongStatus BT_ERROR:[%s]\n\r", stat);
				return 0;											//got something weird, throw the bterror
			}
		}
		else {
			pc.printf("getSongStatus BT_ERROR:[%s]\n\r", stat);
			return 0;												//got something weird, throw the bterror
		}
	}
	else {															//if there is nothing yet
		flushRX();													//get any garbage out of the buffer just in case
		sendCmd(BT_CONNECTAV);										//try to get the song status
		return 0;													//no response yet
	}
}


//toggles the music play/pause status on the bt client, returns the new status (true = playing, false = paused)
//fix me
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
