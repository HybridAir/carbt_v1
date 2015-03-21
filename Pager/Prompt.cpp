//used to display a two item selection prompt (yes/no, etc) on a specific row of the lcd

#include "Prompt.h"

//default constructor, needs the lcd to write to, io to read from, both options (opt1 is the default selected), and the lcd row to write to
Prompt::Prompt(TextLCD& lcdin, io& inoutIn, string opt1, string opt2, int row) : lcd(lcdin), io(inoutIn) {
	this->opt1 = opt1;
	this->opt2 = opt2;
	this->row = row;
}
