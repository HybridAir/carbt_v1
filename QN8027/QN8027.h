#ifndef QN8027_H
#define	QN8027_H

#include <mbed.h>

#define ADDRESS			0x2C		//i2c address for the device

//I2C registers, as named in the datasheet
#define SYSTEM       	0x00
#define CH1				0x01
#define GPLT			0x02
#define REG_XTL			0x03
#define REG_VGA			0x04

//bitmasks
#define CHAN_P1 		0x03
#define TXREQ			0x20
#define MONO			0x10
#define MUTE			0x08
#define IDLE			0x20
#define XINJ			0xC0
#define XSEL			0x80
#define RECAL			0x80

//values
#define NOIDLE			0x30
#define CRYS_DEFAULT	0x00
#define CRYS_INJECT		0x40
#define CRYS_SINGLE		0x80
#define CRYS_DIFF		0xC0
#define CRYS12			0x00
#define CRYS24			0x80


class QN8027 {
    public:
		QN8027(char crysSource, char crysFreq, bool isMono, bool enIdle);
		void Init();
		void getInfo();
		unsigned short getFreq();
		void setFreq(unsigned short freq);
		void setTransmit(bool setTransmit);
		void setMute(bool isMuted);

    private:
		void setBit(char address, char bitmask, char value);
		void setAudioMode(bool isMono);
		void allowIdle(bool allowIdle);
		void setClock(char source);
		void setCrysFreq(char freq);
		char crysSource;
		char crysFreq;
		char isMono;
		char enIdle;

};

#endif
