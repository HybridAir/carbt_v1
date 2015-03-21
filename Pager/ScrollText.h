#ifndef SCROLLTEXT_H
#define	SCROLLTEXT_H

#include <mbed.h>
//#include "rtos.h"
#include "TextLCD.h"
#include "Display.h"
#include <string>
using namespace std;

const int FIRSTDELAY = 1000;


class ScrollText {
    public:
		ScrollText(TextLCD& lcdin, string textIn, int col, int row, int length, int speed);
		void scroll();
    private:
		TextLCD& lcd;
		string textIn;
		Timer time;
		Timer firstWait;
		int col;
		int row;
		int length;
		int speed;
		int curPos;
		bool ready;
		int textLength;
		bool scrolling;
		bool firstRun;
		bool firstWaiting;
};

#endif
