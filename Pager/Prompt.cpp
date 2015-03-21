//used to display a two item selection prompt (yes/no, etc) on a specific row of the lcd

#include "Prompt.h"

//default constructor, needs the lcd to write to, io to read from, both options (opt1 is the default selected), and the lcd row to write to
//options can be 6 characters long MAX
Prompt::Prompt(TextLCD& lcdin,  Display& dispIn, io& inoutIn, string opt1, string opt2, int row) : disp(dispIn), lcd(lcdin), inout(inoutIn) {
	this->opt1 = opt1;
	this->opt2 = opt2;
	this->row = row;
	opt1Length = opt1.length();
	opt2Length = opt2.length();
	askedYet = false;
	space = (14 - (opt1Length + opt2Length));
	opt1Selected = true;
}


//used to display the prompt and to monitor button inputs, must be called continuously
//returns the user's response as a char: 0 = no response yet, 1 = opt1, 2 = opt2
char Prompt::ask() {
	if(!askedYet) {								//if the prompt has not been shown yet
		disp.clearRow(row);						//clear the specified row
		writePrompt(true);						//write the prompt to the lcd, opt1 is selected by default
		askedYet = true;						//we asked the user now, so we don't need to be here ever again
	}
	else {
		if(inout.pressLeft()) {					//if the left button was pressed
			opt1Selected = true;				//select option 1
		}
		else if(inout.pressRight()) {			//if the right button was pressed
			opt1Selected = false;				//select option 2
		}
		else if(inout.pressSelect()) {			//if the select button was pressed
			if(opt1Selected) {					//if option 1 is currently selected
				return 1;						//return 1
			}
			else {
				return 2;						//else, return 2
			}
		}
		writePrompt(opt1Selected);				//update the selected option on the lcd
	}

	return 0;									//if we got here, the user has not responded yet
}


//used to display the prompt to the lcd, needs to know which option to select
void Prompt::writePrompt(bool opt1Select) {
	lcd.locate(0, row);								//set the lcd to the correct row
	if(opt1Select) {								//if opt1 is selected, add the selector icon to it
		lcd.putc(selector);
	}
	else {											//else, leave the selector space blank
		lcd.printf(" ");
	}
	lcd.locate(1, row);
	lcd.printf("%s", opt1.c_str());					//print out opt1 in the correct location
	lcd.locate(opt1Length + 1 + space, row);		//set the lcd to the correct location for printing the next option
	if(!opt1Select) {								//if opt2 is selected, add the selector icon to it
		lcd.putc(selector);
	}
	else {											//else, leave the selector space blank
		lcd.printf(" ");
	}
	lcd.locate(opt1Length + 2 + space, row);		//set the lcd to the correct location for printing opt2
	lcd.printf("%s", opt2.c_str());					//print opt2 to the lcd
}
