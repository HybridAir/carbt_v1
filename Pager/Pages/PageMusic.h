#ifndef PAGEMUSIC_H
#define	PAGEMUSIC_H

#include <mbed.h>


class PageMusic {
    public:
		PageMusic(TextLCD& lcdIn, io& ioIn);
		void showPage();
    private:
		TextLCD& lcd;
		io& inout;
};


#endif
