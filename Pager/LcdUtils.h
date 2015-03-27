#ifndef LCDUTILS_H
#define	LCDUTILS_H

#include <mbed.h>
#include "TextLCD.h"
#include <string>
using namespace std;


class LcdUtils {
	public:
		LcdUtils(TextLCD_SPI_N& lcdIn);
	private:
		TextLCD_SPI_N& lcd;
};

#endif
