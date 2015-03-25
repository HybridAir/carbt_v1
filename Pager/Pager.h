#ifndef PAGER_H
#define	PAGER_H

#include <mbed.h>
#include "io.h"
#include "TextLCD.h"
#include "Display.h"
#include "XS3868.h"
#include "ScrollText.h"
#include "Prompt.h"
#include <string>
using namespace std;

#include "PageMusic.h"

const string BT_ERROR = "Failed to connect, continue without connecting?";


class Pager {
    public:
		Pager(io& ioIn, XS3868& btIn);
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
		Display disp;
		//TextLCD lcd;
		TextLCD_SPI_N lcd;

		PageMusic pMusic;
		bool newPage;


		ScrollText searchText;
		ScrollText connectingText;
		ScrollText connectedText;

};

#endif
