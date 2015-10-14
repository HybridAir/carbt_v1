#ifndef PAGEMUSIC_H
#define	PAGEMUSIC_H

#include <mbed.h>
#include "TextLCD.h"
#include "io.h"
#include "LcdUtils.h"
#include "XS3868.h"


//const char pause[] = {0x12,0x12,0x12,0x12,0x12,0x12,0x12};
//const char nextChar[] = {0x8,0xc,0xe,0xf,0xe,0xc,0x8};
//const char prevChar[] = {0x2,0x6,0xe,0x1e,0xe,0x6,0x2};

class PageMusic {
    public:
		PageMusic(TextLCD_SPI_N& lcdIn, LcdUtils& utilsIn, io& ioIn, XS3868& btIn);
		void showPage(bool first);
    private:
		void showControls();
		void status();
		char btnMon();
		void doMusic();
		void doFreq();
		TextLCD_SPI_N& lcd;
		LcdUtils& utils;
		io& inout;
		XS3868& bt;
		Timer songStatTimer;
		char songStatus;
		bool musicMode;
		bool prevMusicMode;
		//char currentBtn;
};


#endif
