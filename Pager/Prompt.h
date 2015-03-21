#ifndef PROMPT_H
#define	PROMPT_H

#include <mbed.h>
#include "TextLCD.h"
#include "io.h"
#include "Display.h"
#include <string>
using namespace std;

const char selector = 0x7E;								//the right arrow character icon in the lcd

class Prompt {
	public:
		Prompt(TextLCD& lcdin, Display& dispIn, io& inoutIn, string opt1, string opt2, int row);
		char ask();
	private:
		TextLCD& lcd;
		Display& disp;
		io& inout;
		string opt1;
		string opt2;
		int row;
		char opt1Length;
		char opt2Length;
		bool askedYet;
		char space;
		void writePrompt(bool opt1Select);
};

#endif
