#ifndef XS3868_H
#define	XS3868_H

#include <mbed.h>
#include <string>
using namespace std;
#include "BufferedSerial.h"

const string BT_PREFIX = "AT#";
const string BT_SUFFIX = "\r\n";
const string BT_RESET = "CZ";
const string BT_CONNECT = "CC";
const string BT_CONNECTAV = "MI";
const string BT_DISCONNECT = "CD";
const string BT_DISCONNECTAV = "MJ";
const string BT_STATUS = "CY";
const string BT_RECONNECT = "MI";
const string BT_QAVCRP = "MO";
const string BT_FFWDON = "MR";
const string BT_RWDON = "MS";
const string BT_FFRWDOFF = "MT";
const string BT_QA2DP = "MV";
const string BT_PLAYPAUSE = "MA";
const string BT_STOP = "MC";
const string BT_NEXT = "MD";
const string BT_PREV = "ME";
const string BT_PAIR_ON = "CA";
const string BT_PAIR_OFF = "CB";
const string BT_OFF = "VX";

const string BT_BT_O_STATE_CONNECTED1 = "MB";
const string BT_BT_O_STATE_CONNECTED2 = "MA";
const string BT_BT_O_STATE_DISCONNECTED = "MY";
const string BT_BT_O_STATE_PAUSE = "MP";
const string BT_BT_O_STATE_PLAYING = "MR";
const string BT_BT_O_STATE_SPP_ON = "SC";
const string BT_BT_O_STATE_SPP_OFF = "SD";
const string BT_BT_O_STATE_PAIRING_ON = "II";
const string BT_BT_O_STATE_PAIRING_OFF = "IJ2";


class XS3868 {
    public:
		XS3868();
		void connect();
		int getHFPStatus();
		int getSongStatus();
		bool playPause();
		bool connected;
		bool connecting;
		bool bypassBt;					//lets the system know if bluetooth will not be used (bypassed)
    private:
		Timer connectTimer;
		void sendCmd(string command);
		void flushRX();
		bool readStat(char *data);
		char parseHFPStatus(char* stat);
		char parseState(char* stat);
		bool pairing;
		bool disconnect;
		bool disconnecting;
		bool gettingStatus;
		int readLength;
};

#endif
