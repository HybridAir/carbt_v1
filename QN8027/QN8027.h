#ifndef QN8027_H
#define	QN8027_H

#include <mbed.h>

//I2C addresses, as named in the datasheet
#define SYSTEM       	0x00
#define CH1				0x01

//bitmasks
#define CHAN_P1 		0x03
#define AUDIOMODE		0x10

//values
#define MONO			0x10
#define STEREO			0x00


class QN8027 {
    public:
		QN8027();
		unsigned short getFreq();
		void setFreq(unsigned short freq);
		void setAudioMode(bool isMono);
    private:
		void setBit(char address, char bitmask, char value);

};

#endif
