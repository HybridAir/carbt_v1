#ifndef SCROLLTEXT_H
#define	SCROLLTEXT_H

#include <mbed.h>
#include "TextLCD.h"
#include "Display.h"
#include <string>
using namespace std;


class ScrollText {
    public:
		ScrollText(TextLCD& lcdin);
		void scroll(string textIn, int x, int y, int length, int speed, bool direction);
    private:
		TextLCD& lcd;
};

#endif
