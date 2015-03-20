#ifndef SCROLLTEXT_H
#define	SCROLLTEXT_H

#include <mbed.h>
//#include "rtos.h"
#include "TextLCD.h"
#include "Display.h"
#include <string>
using namespace std;


class ScrollText {
    public:
		ScrollText(TextLCD& lcdin, string textIn, int col, int row, int length, int speed, bool left2right);
		void scroll();
    private:
		TextLCD& lcd;
		string textIn;
		int col;
		int row;
		int length;
		int speed;
		bool left2right;
		int curPos;
		bool ready;
};

#endif
