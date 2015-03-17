//this class manages the QN8027 device
//lets see if I can do this
//fuck

#include "QN8027.h"

I2C i2c(D14, D15);			//we need I2C you know

QN8027::QN8027() {

}

//used to set certain bits of a specific address, based off of QNF_SetRegBit
//wants the address, the bitmask, and the value
void QN8027::setBit(char address, char bitmask, char value) {
	char outval;												//prepare the var for use
	i2c.read(address, outval, 8);								//read 8 bits of data in the specified address into outval
	outval = outval & (char)(~bitmask); 						//AND that data with the bitmask NOTed and chared
	outval = outval | (value & bitmask);						//OR that with the ANDed result of value and bitmask
	i2c.write(address, outval, 8);								//write the correctly formatted 8 bits of data out to the device

	//EXAMPLE: we want to MUTE the device
	//use address 0x00, bit position 4, and a value of 1 (00010000)
	//setBit(0x00, 0x10, 0x10);

	//i2cread = 00000011 for example
	//out = 00000011 & 11101111 = 11
	//out = 11 | (00010000 & 00010000) = 00010011
	//i2cwrite 00010011
	//done
}
