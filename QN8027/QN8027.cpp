//class used to manage/operate the QN8027 device
//TODO: add RDS support (ex: carbt_v1)

#include "QN8027.h"

I2C i2c(D14, D15);			//we need I2C you know

extern Serial pc;

QN8027::QN8027(char crysSource, char crysFreq, bool isMono, bool enIdle) {
	this->crysSource = crysSource;
	this->crysFreq = crysFreq;
	this->isMono = isMono;
	this->enIdle = enIdle;
	i2c.frequency(400);
	Init();
	getInfo();
}


//used for debug
void QN8027::getInfo() {
	char outval[8];												//prepare the var for use
	i2c.read((char)0x06, outval, 8);
	//pc.printf("asdfd");
	//pc.printf("%d", outval[1] );
	//pc.printf("%x", outval[0]);
}


//used to reset, recalibrate, and initialize the device
void QN8027::Init() {
	char cmd[1];
	cmd[0] = 0x80;
	int x = i2c.write(SYSTEM, cmd, 1);						//resets to all default settings (SWRST = 1)
	pc.printf(x);
	wait_ms(20);									//wait for it to reset
	setClock(crysSource);
	setCrysFreq(crysFreq);
	setAudioMode(isMono);
	allowIdle(enIdle);
	setBit(SYSTEM, RECAL, RECAL);						//reset the FSM (RECAL = 1)
	setBit(SYSTEM, RECAL, (char)0x00);						//recalibrate the FSM (RECAL = 0)
	wait_ms(20);									//wait for calibrations to complete
}


//used to set certain bits to a specific address, based off of QNF_SetRegBit
//wants the address, the bitmask, and the value
//wait cant I use this for everything
void QN8027::setBit(char address, char bitmask, char value) {
	char outval[1];												//prepare the var for use
	i2c.read(address, outval, 1);								//read 8 bits of data in the specified address into outval
	outval[0] = outval[0] & (char)(~bitmask); 						//AND that data with the bitmask NOTed and chared
	outval[0] = outval[0] | (value & bitmask);						//OR that with the ANDed result of value and bitmask
	i2c.write(address, outval, 1);								//write the correctly formatted 8 bits of data out to the device

	//EXAMPLE: we want to MUTE the device
	//use address 0x00, bit position 4, and a value of 1 (00010000)
	//setBit(0x00, 0x10, 0x10);

	//i2cread = 00000011 for example
	//out = 00000011 & 11101111 = 11
	//out = 11 | (00010000 & 00010000) = 00010011
	//i2cwrite 00010011
	//done
}


//used to get the frequency currently set, based off QNF_GetCh
//returns the frequency formatted like: 10800 for 108.00MHz
unsigned short QN8027::getFreq() {
	char part1[1];													//holds the first 2 bits of the 10 bit channel, "part1", since they're stored in another address
	i2c.read(SYSTEM, part1, 1);									//read all the bytes from address SYSTEM (0x00) out into part1
	part1[0] = part1[0] & CHAN_P1;									//AND it with the CHAN_P1 bitmask (0x03 = 00000011), since the last two bits are the ones we want
	unsigned short outval = part1[0];								//get the output ready, we are done with part1

	char part2[1];													//holds the last 8 bits of the 10 bit channel, all stored in their own address
	i2c.read(CH1, part2, 1);									//read all the bytes from address CH1 (0x00) out into part2
	outval = (outval << 8) + part2[0];								//shift the current outval left 8 bits, and add part2, so we get a 10bit output
	return (outval*5) + 7600;									//convert the fixed frequency and return it

	//EXAMPLE: the frequency is currently set to 98.0 MHZ
	//i2cread = 00000001 = part1
	//part1 = 00000001 & 00000011 = 00000001 = outval

	//i2cread = 10111000 = part2
	//outval = 100000000 + 10111000 = 110111000
	//return 440*5 + 7600 = 9800 this is correct
}


//used to set the frequency, based off QNF_SetCh
//give it a frequency formatted like: 10800 for 108.00MHz
void QN8027::setFreq(unsigned short freq) {

	freq = (freq-7600)/5;										//convert the frequency because reasons
	char part2[1];
	part2[0] = (char)freq;										//cut off the first 8 bits, so we are left with only 8, part2
	i2c.write(CH1, part2, 1);									//write part2 to CH1

	//this all might be skippable becuase of setBit above
	char part1[1];
	i2c.read(SYSTEM, part1, 1);									//read all the bytes from address SYSTEM (0x00) out into part1
	//this is necessary because there are other bits within that address that we want to keep the same
	part1[0] = part1[0] & ~CHAN_P1;									//AND it with the CHAN_P1 bitmask (0x03 = 00000011), since the last two bits are the ones we want
	part1[0] = part1[0] | ((char)(freq >> 8) & CHAN_P1);				//OR part1 with the freq that has been shifted right 8, and ANDed by the CHAN_P1 bitmask
	i2c.write(SYSTEM, part1, 1);								//write part1 to SYSTEM

	//EXAMPLE: we want to set the frequency to 98.0 MHZ
	//when formatted, it equals 9800 or 10011001001000

	//freq = 440 or 110111000
	//part2 = 184 becuase the 1 in front was chopped off
	//i2cwrite(CH1, 10111000, 8)

	//i2cread = 00000011 for example
	//part1 = 00000011 & 11111100 = 0
	//part1 = 0 | (1 & 00000011 = 1) = 1
	//i2cwrite(SYSTEM, 00000001, 8) this is correct

}


//used to set the tx status (IDLE or transmitting)
//use it to save power
void QN8027::setTransmit(bool setTransmit) {
	if(setTransmit) {
		setBit(SYSTEM, TXREQ, TXREQ);
	}
	else {
		setBit(SYSTEM, TXREQ, 0x00);
	}
}


//used to set the fm audio tx to Mono or Stereo mode, based off QNF_SetAudioMono
//needs a boolean value
void QN8027::setAudioMode(bool isMono) {
	if(isMono) {
		setBit(SYSTEM, MONO, MONO);
	}
	else {
		setBit(SYSTEM, MONO, 0x00);
	}
}


//used to set the fm audio tx to Mono or Stereo mode, based off QNF_SetAudioMono
void QN8027::setMute(bool isMuted) {
	if(isMuted) {
		setBit(SYSTEM, MUTE, MUTE);
	}
	else {
		setBit(SYSTEM, MUTE, 0x00);
	}
}


//used to allow the device to go into IDLE mode
//disable if you want the device to always be transmitting, even when there is no sound
void QN8027::allowIdle(bool allowIdle) {
	if(allowIdle) {
		setBit(GPLT, IDLE, IDLE);
	}
	else {
		setBit(GPLT, IDLE, NOIDLE);
	}
}


//used to set the device's clock source
//default is CRYS_DEFAULT, crystal on both pins
void QN8027::setClock(char source) {
	setBit(REG_XTL, XINJ, source);
}


//used to set the crystal frequency
void QN8027::setCrysFreq(char freq) {
	setBit(REG_VGA, XSEL, freq);
}


