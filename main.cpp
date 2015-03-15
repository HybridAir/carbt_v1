//watch me as I try to figure out what the hell this thing is going to do
#include <mbed.h>
#include "Display.h"
#include "io.h"
#include "XS3868.h"

Serial pc(USBTX, USBRX); // tx, rx

Display disp;
io inout;
XS3868 bt;



int main() 
{

	pc.printf("Music Control\n");

	disp.init();

	bt.init();
	bt.connect();

	inout.doled();
}
