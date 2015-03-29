#ifndef PROMPT_H
#define	PROMPT_H

#include <mbed.h>
#include "TextLCD.h"
#include "io.h"
#include "LcdUtils.h"
#include "Pager.h"
#include <string>
using namespace std;

const char selector = 0x7E;								//the right arrow character icon in the lcd

class Prompt {
	public:
		Prompt(TextLCD_SPI_N& lcdin, LcdUtils& utilsIn, io& inoutIn, string opt1, string opt2, int row);
		char ask();
	private:
		TextLCD_SPI_N& lcd;
		LcdUtils& utils;
		io& inout;
		string opt1;
		string opt2;
		int row;
		char opt1Length;
		char opt2Length;
		bool askedYet;
		char space;
		bool opt1Selected;
		void writePrompt(bool opt1Select);
};

#endif
