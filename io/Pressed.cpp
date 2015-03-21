//used to detect falling edge button presses

#include "Pressed.h"

//default constructor, needs IO for pinnames, and the specific debouncein object for reading buttons
Pressed::Pressed(DebounceIn& btnIn) : btn(btnIn) {
	currentlyPressed = false;
}


//returns whether the button was just pressed (falling edge/high then low), must be called continuously
bool Pressed::btnPress() {
	if(btn.read()) {											//if the button is currently high
		currentlyPressed = true;								//the button is currently being held down
		return false;											//but has not yet been released
	}
	else if(!btn.read() && currentlyPressed) {					//else if the button is currently low, and was previously held down
		currentlyPressed = false;								//the button is no longer being held down
		return true;											//the button has been released, return true
	}
	else if(!btn.read()) {										//if the button is just low
		return false;											//button has not been pressed yet
	}
	return 0;													//makes the compiler happy
}
