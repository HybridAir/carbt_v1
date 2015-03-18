//class used to manage/operate the QN8027 device
//TODO: add RDS support (ex: carbt_v1)

#include "QN8027.h"

extern I2C i2c;

QN8027::QN8027(char crysSource, char crysFreq, bool isMono, bool enIdle) {
	this->crysSource = crysSource;
	this->crysFreq = crysFreq;
	this->isMono = isMono;
	this->enIdle = enIdle;


    printf("Searching for I2C devices...");

    int count = 0;
    for (int address=0; address<256; address+=2) {
        if (!i2c.write(address, NULL, 0)) { // 0 returned is ok
            printf(" - I2C device found at address 0x%02X\n", address);
            count++;
        }
    }
    printf("%d devices found\n", count);

//	char wrout[] = {0x04};
//	int reg = i2c.write(ADDRESS, wrout, 1);
//	printf("reg:");
//	printf("%d", reg);
//
//
//	char redin;
//	int rin = i2c.read(ADDRESS, &redin, 1);
//		printf("rin:");
//		printf("%d,", rin);
//		printf("%x,", redin);


		//printf("readin:%x", readIn((char)0x04));
    //printf("freq:%d", readIn(getFreq()));

    //printf("freq:%u", getFreq());




	Init();


	//writeBits(REG_XTL, (char)0xC0, (char)0xC0);
	//writeBits(REG_XTL, XINJ, CRYS_DIFF);

			printf("readit:%u", getFreq());
			//setTransmit(true);
			printf("status:%x\n", readIn((char)0x07));
			//wait_ms(50);
				setTransmit(true);
				//setMute(false);
				//wait_ms(500);
				//wait(1);
			//allowIdle(true);
				//writeBits((char)0x10, (char)0x80, (char)0x80);
				//writeBits((char)0x10, (char)0x80, (char)0x00);
			while(1) {
	//printf("status:%x\n", readIn((char)0x07));
	//writeBits((char)0x10, (char)0x80, (char)0x80);
					//writeBits((char)0x10, (char)0x80, (char)0x00);
	//printf("status:%x\n", readIn((char)0x10));
	//setTransmit(false);
				printf("status:%x\n", readIn((char)0x07));
	wait(1);
			}
}


//used to write 1 byte of data directly to the device
void QN8027::writeOut(char reg, char data) {
	char dataOut[2];								//enough room for the register and data
	dataOut[0] = reg;
	dataOut[1] = data;

	i2c.write(ADDRESS, dataOut, 2);					//write out 2 bytes of data (register and data) to the specified device address
}


//used to read in from the device, needs the register to read from, and returns the 1 byte data as a char
char QN8027::readIn(char reg) {
	//char regOut = reg;
	i2c.write(ADDRESS, &reg, 1);					//first we write the specified register to the device so it knows where to read from

	char dataIn;
	int x = i2c.read(ADDRESS, &dataIn, 1);
	return dataIn;
}


//used to write specific bits of data to the device, it merges the current data with the new data
//needs the register, bitmask, and data to merge in
void QN8027::writeBits(char reg, char bitmask, char data) {
	char output = readIn(reg);
	output = output & (char)(~bitmask); 						//AND that data with the bitmask NOTed and chared
	output = output | (data & bitmask);							//OR that with the ANDed result of value and bitmask
	writeOut(reg, output);

	//EXAMPLE: we want to MUTE the device
	//use address 0x00, bit position 4, and a value of 1 (00010000)
	//setBit(0x00, 0x10, 0x10);

	//i2cread = 00000011 for example
	//out = 00000011 & 11101111 = 11
	//out = 11 | (00010000 & 00010000) = 00010011
	//i2cwrite 00010011
	//done


}


//used to reset, recalibrate, and initialize the device
void QN8027::Init() {
	char cmd = 0x80;
	writeOut(SYSTEM, cmd);						//resets to all default settings (SWRST = 1)
	wait_ms(100);									//wait for it to reset

	//setAudioMode(isMono);
	//allowIdle(enIdle);
	//setFreq(9400);
	//	wait(1);

	writeBits(SYSTEM, RECAL, RECAL);
	writeBits(SYSTEM, RECAL, (char)0x00);
	wait_ms(100);									//wait for calibrations to complete
	setFreq(9400);
	writeBits(SYSTEM, RECAL, RECAL);
		writeBits(SYSTEM, RECAL, (char)0x00);
		wait_ms(100);
			//wait(1);
}



//used to get the frequency currently set, based off QNF_GetCh
//returns the frequency formatted like: 10800 for 108.00MHz
unsigned short QN8027::getFreq() {
	char part1;													//holds the first 2 bits of the 10 bit channel, "part1", since they're stored in another address
	//i2c.read(SYSTEM, part1, 1);									//read all the bytes from address SYSTEM (0x00) out into part1
	part1 = readIn(SYSTEM);
	part1 = part1 & CHAN_P1;									//AND it with the CHAN_P1 bitmask (0x03 = 00000011), since the last two bits are the ones we want
	unsigned short outval = part1;								//get the output ready, we are done with part1
	//printf("part1:%x,", part1);

	char part2;													//holds the last 8 bits of the 10 bit channel, all stored in their own address
	//i2c.read(CH1, part2, 1);									//read all the bytes from address CH1 (0x00) out into part2
	part2 = readIn(CH1);
	outval = (outval << 8) + part2;								//shift the current outval left 8 bits, and add part2, so we get a 10bit output
	//printf("part2:%x,", part2);
	//printf("get:%u", outval);
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
	char part2;
	part2 = (char)freq;										//cut off the first 8 bits, so we are left with only 8, part2
	//i2c.write(CH1, part2, 1);									//write part2 to CH1
	writeOut(CH1, part2);
	printf("part2:%x,", part2);

	//this all might be skippable becuase of setBit above
	char part1;
	//i2c.read(SYSTEM, part1, 1);									//read all the bytes from address SYSTEM (0x00) out into part1
	//this is necessary because there are other bits within that address that we want to keep the same
	//part1[0] = part1[0] & ~CHAN_P1;									//AND it with the CHAN_P1 bitmask (0x03 = 00000011), since the last two bits are the ones we want
	part1 = ((char)(freq >> 8) & CHAN_P1);				//OR part1 with the freq that has been shifted right 8, and ANDed by the CHAN_P1 bitmask
	//i2c.write(SYSTEM, part1, 1);								//write part1 to SYSTEM
	writeBits(SYSTEM, CHAN_P1, part1);
	printf("part1:%x;", part1);

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
		writeBits(SYSTEM, TXREQ, TXREQ);
	}
	else {
		writeBits(SYSTEM, TXREQ, 0x00);
	}
}


//used to set the fm audio tx to Mono or Stereo mode, based off QNF_SetAudioMono
//needs a boolean value
void QN8027::setAudioMode(bool isMono) {
	if(isMono) {
		writeBits(SYSTEM, MONO, MONO);
	}
	else {
		writeBits(SYSTEM, MONO, 0x00);
	}
}


//used to set the fm audio tx to Mono or Stereo mode, based off QNF_SetAudioMono
void QN8027::setMute(bool isMuted) {
	if(isMuted) {
		writeBits(SYSTEM, MUTE, MUTE);
	}
	else {
		writeBits(SYSTEM, MUTE, 0x00);
	}
}


//used to allow the device to go into IDLE mode
//disable if you want the device to always be transmitting, even when there is no sound
void QN8027::allowIdle(bool allowIdle) {
	if(allowIdle) {
		writeBits(GPLT, IDLE, IDLE);
	}
	else {
		writeBits(GPLT, IDLE, NOIDLE);
	}
}


//used to set the device's clock source
//default is CRYS_DEFAULT, crystal on both pins
void QN8027::setClock(char source) {
	writeBits(REG_XTL, XINJ, source);
}


//used to set the crystal frequency
void QN8027::setCrysFreq(char freq) {
	writeBits(REG_VGA, XSEL, freq);
}


