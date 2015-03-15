//used for controlling the OVC3860 based XS3868 board

#include "XS3868.h"

Serial sbt(PA_11, PA_12); // tx, rx

XS3868::XS3868() {

}

void ;XS3868::init() {
	sbt.baud(115200);
	sbt.printf("AT#CZ\n\r");
	wait(3);
}

void XS3868::connect() {

	sbt.printf("AT#CC\r\n");
}
