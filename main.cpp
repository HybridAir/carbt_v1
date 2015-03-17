//watch me as I try to figure out what the hell this thing is going to do
#include <mbed.h>
#include "Display.h"
#include "io.h"
#include "XS3868.h"
#include "QN8027.h"

Serial pc(USBTX, USBRX); // tx, rx
QN8027 fm(CRYS_DEFAULT, CRYS12, false, false);
Display lcd;
io inout;
XS3868 bt;

int main() 
{

	pc.printf("Music Control\n");

	bt.init();
	bt.connect();

	inout.doled();
}
