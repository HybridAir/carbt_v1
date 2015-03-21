#ifndef PRESSED_H
#define	PRESSED_H

#include <mbed.h>
//#include "io.h"
#include "DebounceIn.h"

class Pressed {
	public:
		Pressed(DebounceIn& btn);
		bool btnPress();
	private:
		DebounceIn& btn;
		bool currentlyPressed;
};

#endif
