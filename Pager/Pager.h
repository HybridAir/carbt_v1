#ifndef PAGER_H
#define	PAGER_H

#include <mbed.h>
#include "io.h"
#include "TextLCD.h"
#include "Display.h"
#include "ScrollText.h"
#include "Prompt.h"
#include <string>
using namespace std;

const string BT_ERROR = "Failed to connect, continue without connecting?";


class Pager {
    public:
		Pager(io& ioIn);
		void init();
		void showTitle();
		void search();
		void connecting();
		void connected();
		void bterror();
		bool askBypassBt();
    private:
		io& inout;
		Display disp;
		TextLCD lcd;
		ScrollText searchText;
		ScrollText connectingText;
		ScrollText connectedText;
		ScrollText bterrorText;
};

#endif
