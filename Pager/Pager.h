#ifndef PAGER_H
#define	PAGER_H

#include <mbed.h>
#include "io.h"
#include "TextLCD.h"
#include "LcdUtils.h"
#include "XS3868.h"
#include "ScrollText.h"
#include "Prompt.h"
#include <string>
using namespace std;

#include "PageMusic.h"

const string BT_ERROR = "Failed to connect, continue without connecting?";

//const char top1[] = {0x1, 0x1, 0x3, 0x3, 0x7, 0x7, 0x3, 0x1};
//const char top2[] = {0x10, 0x10, 0x18, 0x18, 0x1c, 0x1c, 0x18, 0x10};
//const char left2[] = {0x18, 0x1c, 0x1e, 0x1e, 0x1e, 0x18, 0x0, 0x0};
//const char left1[] = {0x0, 0x0, 0x1, 0x3, 0x7, 0xf, 0x1e, 0x0};
//const char right2[] = {0x3, 0x7, 0xf, 0xf, 0xf, 0x3, 0x0, 0x0};
//const char right1[] = {0x0, 0x0, 0x10, 0x18, 0x1c, 0x1e, 0xf, 0x0};


class Pager {
    public:
		Pager(TextLCD_SPI_N& lcdIn, io& ioIn, XS3868& btIn);
		void init();
		void showTitle();
		void doPage();
		void doMenu();

		void search();
		void connecting();
		void connected();
		void bterror();
		bool askBypassBt();

		char activePage;
		char oldPage;
    private:
		io& inout;
		XS3868& bt;
		//Display disp;
		//TextLCD lcd;
		TextLCD_SPI_N& lcd;
		LcdUtils utils;
		PageMusic pMusic;
		bool newPage;


		ScrollText searchText;
		ScrollText connectingText;
		ScrollText connectedText;

};

#endif
