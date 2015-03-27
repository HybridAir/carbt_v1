//manages lcd utility stuff like custom characters/display, powerup/error? pages, text centering, etc

#include "LcdUtils.h"


//default constructor, needs the lcd object
LcdUtils::LcdUtils(TextLCD_SPI_N& lcdIn) : lcd(lcdIn) {

}
