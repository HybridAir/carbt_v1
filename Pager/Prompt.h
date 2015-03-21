#ifndef PROMPT_H
#define	PROMPT_H

#include <mbed.h>
#include "TextLCD.h"
#include "io.h"
#include "Display.h"
#include <string>
using namespace std;

class Prompt {
	public:
		Prompt(TextLCD& lcdin, io& inoutIn, string opt1, string opt2, int row);
	private:
		TextLCD& lcd;
		io& inout;
		string opt1;
		string opt2;
		int row;
};

#endif
