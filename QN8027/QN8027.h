#ifndef QN8027_H
#define	QN8027_H

#include <mbed.h>

//I2C addresses, as named in the datasheet
#define SYSTEM       	0x00
#define CH1				0x01

//bitmasks
#define CHAN_P1 		0x03
#define MONO			0x10
#define TXREQ			0x20

//values
#define MONOMODE		0x10
#define STEREOMODE		0x00
#define IDLE			0x20
#define ACTIVE			0x00


class QN8027 {
    public:
		QN8027();
		void Init();
		unsigned short getFreq();
		void setFreq(unsigned short freq);
		void setAudioMode(bool isMono);
		void setIdle(bool setIdle);
    private:
		void setBit(char address, char bitmask, char value);

};

#endif
