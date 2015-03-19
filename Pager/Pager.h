#ifndef PAGER_H
#define	PAGER_H

#include <mbed.h>
#include "TextLCD.h"
#include "Display.h"
#include <string>
using namespace std;


class Pager {
    public:
		Pager();
		void init();
    private:
		Display disp;
		TextLCD lcd;
		//TextLCD lcd(D8, D9, D4, D5, D6, D7);
		//Display disp(lcd);
};

#endif
