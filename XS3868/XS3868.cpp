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

void callback();
void callback() {
    // Note: you need to actually read from the serial to clear the RX interrupt
    //response = true;
	pc.printf("respoinse\r\n");
	pc.printf("get[%c]\n", sbt.getc());

					//char str[4];								//create an arry to store it in
					//sbt.scanf("%s", str);						//get the response in

					//pc.printf("Stat got [%s]\r\n", str);		//temp debug
}


//handles connecting to the bluetooth client, call this frequently
//might have to make it return the status later
//give this access to pager?
void XS3868::connect() {
	if(!connecting) {
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
		//z.start();
		//sbt.attach(&callback);
	}



	if(gettingStatus) {										//first part of connecting is getting the device's current status
		if(connectTimer.read_ms() >= 1000) {				//try to get the device status every 100 ms if we don't have it yet
			flushRX();										//get any garbage out of the buffer just in case
			sendCmd(BT_STATUS);								//send the status request command
			connectTimer.reset();
		}
		else {
			char stat[3];
			if(readStat(stat)) {
				if(parseStatus(stat) == '1') {
					pc.printf("ready to pair\r\n");
				}
			}

		}
//			char counter = 0;
//							char str[3];
//
//			while(1) {
//
//				while(sbt.readable()) {
//					char in = sbt.getc();
//					if(in == '\r' || in == '\n') {
//						if(counter > 0) {
//							pc.printf("Stat got [%s]\r\n", str);		//temp debug
//						}
//					}
//					else {
//						str[counter] = in;
//						pc.printf("charin [%c]\r\n", str[counter]);
//												counter++;
//					}
//
//					//pc.putc(sbt.getc());
//
//
//					//char str[4];								//create an arry to store it in
//					//sbt.scanf("%c%c", str[0], str[1]);						//get the response in
//					//pc.printf("Stat got [%s]\r\n", str);		//temp debug
//				}
//
//			}

		}

//		if(sbt.readable()) {						//if we got a response
//				char str[4];								//create an arry to store it in
//				sbt.scanf("%s", str);						//get the response in
//
//				pc.printf("Stat got [%s]\r\n", str);		//temp debug
//
//				switch(parseStatus(str)) {					//parse the status
//					case '1':								//client is not connected yet
//						pc.printf("Ready\r\n");
//						pairing = true;						//device is ready to pair with client
//						break;
//					case '2':
//						pc.printf("Connecting\r\n");
//						pairing = true;						//shouldn't ever get here, might as well go to pairing anyways
//						break;
//					case '3':
//						pc.printf("Connected\r\n");
//						disconnecting = true;				//need to disconnect the device before pairing for stability or something, idk
//						break;
//					case '4':
//						pc.printf("Parse error\r\n");
//						//disconnecting = true;				//something wrong happened
//				}
//
//				gettingStatus = false;						//done getting the status
//				flushRX();									//clear out anything leftover in the rx buffer
//				connectTimer.reset();						//reset the timer
//		}
	//}
	else if(disconnecting) {
		if(connectTimer.read_ms() >= 100) {				//try to get the client to disconnect every 100 ms
			sendCmd(BT_DISCONNECTAV);
			sendCmd(BT_DISCONNECT);
			connectTimer.reset();						//reset the 100 ms timer
		}
		else if(sbt.readable()) {						//if we got a response
			char str[4];								//create an arry to store it in
			//sbt.scanf("%s", str);						//get the response in
			pc.printf("Discon got [%s]\r\n", str);		//temp debug
			while(1) {}
		}
	}



//	if(disconnecting) {
//		if(disconnect) {
//			sendCmd(BT_DISCONNECTAV);
//			sendCmd(BT_DISCONNECT);
//			disconnect = false;
//		}
//		else {
//			if(sbt.readable()) {
//				while(sbt.readable()) {
//					pc.putc(sbt.getc());
//				}
//				disconnecting = false;
//				pairing = true;
//			}
//		}
//	}
	//pairing = true;

	while(pairing) {				//change this to if so we can do something else while this works
//		if(newResponse) {					//if we got something from the device
//			//check it
//			//if it's good
//			pairing = false;
//		}

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


//extracts and returns the State Query status number, use it to get the bluetooth connection status
//call it continuously until you get something not 0
//0: no response, device is possibly offline, keep trying
//1: Ready
//2: Connecting
//3: Connected
//4: bad response, device sent garbage or a response to a different command


//actually, pass this the string we got from outside this instead
//this will only parse it
char XS3868::parseStatus(char* stat) {

		if(stat[0] == 'M' && stat[1] == 'G') {			//make sure it's the correct response type
			if(stat[2] == '1') {
				return '1';
			}
			else if(stat[2] == '2') {
				return '2';
			}
			else if (stat[2] > '2') {
				connected = true;
				return '3';
			}
		}
		else if(stat[0] == 'I' && stat[1] == 'V') {
			connected = true;
			return '3';
		}
		else {
			pc.printf("BT_ERROR:%s\n\r", stat);					//we got a bad response, print out the error over serial
			return '4';												//bad response code
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
