//watch me as I try to figure out what the hell this thing is going to do
#include <mbed.h>
#include "Pager.h"
//#include "Display.h"
#include "io.h"
#include "XS3868.h"
//#include "QN8027.h"											//temporarily disabled


Serial sbt(PA_11, PA_12); // tx, rx
Serial pc(USBTX, USBRX); // tx, rx
I2C i2c(D14, D15);

//QN8027 fm(CRYS_DEFAULT, CRYS24, false, false);				//temporarily disabled
//Display lcd;					//gets the lcd going
Pager pager;
io inout;
XS3868 bt;

int main() 
{
	//lcd.init();				//go through the connection process
//wait(1);
//pager.init();

	while(1) {
		bt.init();			//initialize the device, includes conencting to the client

	}


}
