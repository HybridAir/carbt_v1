//used to manage buttons and leds and sensors and stuff, nothing LCD related
//TODO: button led brightness control, light sensor, temp sensor, car voltage, beeper

#include "io.h"

extern Serial pc;

	DebounceIn Left(LEFT);
	DebounceIn Select(SELECT);
	DebounceIn Right(RIGHT);
	DebounceIn Menu(MENU);

	Pressed leftBtn(Left);
	Pressed selectBtn(Select);
	Pressed rightBtn(Right);
	Pressed menuBtn(Menu);

	DigitalOut btnLed(BTN_LED);
	PwmOut conLed(CON_LED);

io::io() {
	wait_ms(250);								//wait for everything to power up
}

void io::init() {
	Left.set_debounce_us(5000);
	Select.set_debounce_us(5000);
	Right.set_debounce_us(5000);
	Menu.set_debounce_us(5000);

	leftPressed = false;
	conLedPwm = 0;								//start the connection led at 0 brightness
	readyConPwm = true;							//ready to update the connection led pwm
	conIncreasing = true;						//it will be increasing
	timeConLed.start();							//start the timer now (might not need to)
}

bool io::pressSelect() {
	return selectBtn.btnPress();
}

bool io::pressLeft() {
	return leftBtn.btnPress();
}

bool io::pressRight() {
	return rightBtn.btnPress();
}

bool io::pressMenu() {
	return menuBtn.btnPress();
}

bool io::btnPressAll() {
	if(leftBtn.btnPress() || selectBtn.btnPress() || rightBtn.btnPress() || menuBtn.btnPress()) {
		return true;
	}
	else {
		return false;
	}
}

bool io::readSelect() {
	return Select.read();
}

bool io::readRight() {
	return Right.read();
}

bool io::readMenu() {
	return Menu.read();
}

bool io::btnReadAll() {
	if(Left.read() || Select.read() || Right.read() || Menu.read()) {
		return true;
	}
	else {
		return false;
	}
}


//controls the connection led, needs the XS3868 status number, and must be called continuously if you want good fading
void io::connectionLed(char status) {
	if(status == 0 || status == 1) {							//if the XS3868 is searching or connecting
		//the following is used to fade the connection led on and off, becuase it looks cool
		if(readyConPwm) {										//if we're ready to change the pwm
			if(conIncreasing) {									//if the pwm value is currently increasing
				conLedPwm += 0.05f;								//add .05 to it
				if(conLedPwm > 1) {								//if the pwm value is over 1
					conIncreasing = false;						//we need to decrease the pwm value now
				}
			}
			else {												//if the pwm value is decreasing
				conLedPwm -= 0.05f;								//subtract .05 from it
				if(conLedPwm < 0) {								//if the pwm value is less than 0
					conIncreasing = true;						//start increasing the pwm value
				}
			}
			readyConPwm = false;								//need to wait until we can change the pwm again
			timeConLed.start();									//start the wait timer
		}
		else if(timeConLed.read_ms() >= 20) {					//wait until it has been 10 ms
			readyConPwm = true;									//ready to change the pwm now
			timeConLed.stop();									//stop and reset the timer
			timeConLed.reset();
		}
		conLed = conLedPwm;										//update the led with the new pwm
	}
	else if(status == 2) {										//if it's connected
		conLed = 1;												//keep the led on
	}
	else {														//if we got an error or want to bypass or something
		conLed = 0;												//keep the led off
	}
}

