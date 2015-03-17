//this class manages the QN8027 device
//lets see if I can do this
//fuck

#include "QN8027.h"

I2C i2c(D14, D15);			//we need I2C you know

QN8027::QN8027() {

}

//used to set certain bits to a specific address, based off of QNF_SetRegBit
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

//used to get the frequency currently set on the device, based off QNF_GetCh
//returns the frequency like: 10800 for 108.00MHz
unsigned short QN8027::getFreq() {
	char part1;													//holds the first 2 bits of the 10 bit channel, "part1", since they're stored in another address
	i2c.read(SYSTEM, part1, 8);									//read all the bytes from address SYSTEM (0x00) out into part1
	part1 = part1 & CHAN_P1;									//AND it with the CHAN_P1 bitmask (0x03 = 00000011), since the last two bits are the ones we want
	unsigned short outval = part1;									//get the output ready, we are done with part1

	char part2;													//holds the last 8 bits of the 10 bit channel, all stored in their own address
	i2c.read(CH1, part2, 8);									//read all the bytes from address CH1 (0x00) out into part2
	outval = (outval << 8) + part2;								//shift the current outval left 8 bits, and add part2, so we get a 10bit output
	return (outval*5) + 7600;									//convert the fixed frequency and return it

	//add an example here
}
