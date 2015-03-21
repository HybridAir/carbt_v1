#ifndef PAGER_H
#define	PAGER_H

#include <mbed.h>
#include "TextLCD.h"
#include "Display.h"
#include "ScrollText.h"
#include <string>
using namespace std;

const string BT_ERROR = "Failed to connect, continue without connecting?";


class Pager {
    public:
		Pager();
		void init();
		void search();
		void connecting();
		void connected();
		void bterror();
    private:
		Display disp;
		TextLCD lcd;
		ScrollText searchText;
		ScrollText connectingText;
		ScrollText connectedText;
		ScrollText bterrorText;
};

#endif
