/* mbed TextLCD Library, for a 4-bit LCD based on HD44780
 * Copyright (c) 2014, WH
 *               2014, v01: WH, Extracted from TextLCD.h as of v14
 *               2014, v02: WH, Added AC780 support, added I2C expander modules, fixed setBacklight() for inverted logic modules. Fixed bug in LCD_SPI_N define
 *               2014, v03: WH, Added LCD_SPI_N_3_8 define for ST7070
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#ifndef MBED_TEXTLCDCONFIG_H
#define MBED_TEXTLCDCONFIG_H

//Select hardware interface options to reduce memory footprint (multiple options allowed)
#define LCD_I2C        1           /* I2C Expander PCF8574/MCP23008 */
#define LCD_SPI        1           /* SPI Expander SN74595          */
#define LCD_I2C_N      1           /* Native I2C bus     */
#define LCD_SPI_N      1           /* Native SPI bus     */
#define LCD_SPI_N_3_8  1           /* Native SPI bus     */
#define LCD_SPI_N_3_9  1           /* Native SPI bus     */
#define LCD_SPI_N_3_10 1           /* Native SPI bus     */
#define LCD_SPI_N_3_16 1           /* Native SPI bus     */
#define LCD_SPI_N_3_24 1           /* Native SPI bus     */

//Select options to reduce memory footprint (multiple options allowed)
#define LCD_UDC        1           /* Enable predefined UDC example*/                
#define LCD_PRINTF     1           /* Enable Stream implementation */                

//Pin Defines for I2C PCF8574/PCF8574A or MCP23008 and SPI 74595 bus expander interfaces
//LCD and serial portexpanders should be wired accordingly 
//
//Select Hardware module (one option only)
#define DEFAULT        1
#define ADAFRUIT       0
#define DFROBOT        0
#define YWROBOT        0
#define GYLCD          0
#define SYDZ           0

//Select Hardware module (one option only)
#if (DEFAULT==1)
//Definitions for default (WH) mapping between serial port expander pins and LCD controller
//This hardware supports the I2C bus expander (PCF8574/PCF8574A or MCP23008) and SPI bus expander (74595) interfaces
//See https://mbed.org/cookbook/Text-LCD-Enhanced
//
//Note: LCD RW pin must be connected to GND
//      E2 is used for LCD40x4 (second controller)
//      BL may be used to control backlight
#define D_LCD_PIN_D4   0
#define D_LCD_PIN_D5   1
#define D_LCD_PIN_D6   2
#define D_LCD_PIN_D7   3
#define D_LCD_PIN_RS   4
#define D_LCD_PIN_E    5
#define D_LCD_PIN_E2   6
#define D_LCD_PIN_BL   7

#define D_LCD_PIN_RW   D_LCD_PIN_E2

//Select I2C Portexpander type (one option only)
#define PCF8574        1
#define MCP23008       0

//Inverted Backlight control
#define BACKLIGHT_INV  0
#endif

#if (ADAFRUIT==1)
//Definitions for Adafruit i2cspilcdbackpack mapping between serial port expander pins and LCD controller
//This hardware supports both an I2C expander (MCP23008) and an SPI expander (74595) selectable by a jumper.
//Slaveaddress may be set by solderbridges (default 0x40). SDA/SCL has pullup Resistors onboard.
//See http://www.ladyada.net/products/i2cspilcdbackpack
//
//Note: LCD RW pin must be kept LOW
//      E2 is not available on this hardware and so it does not support LCD40x4 (second controller)
//      BL is used to control backlight
#define D_LCD_PIN_0    0
#define D_LCD_PIN_RS   1
#define D_LCD_PIN_E    2
#define D_LCD_PIN_D4   3
#define D_LCD_PIN_D5   4
#define D_LCD_PIN_D6   5
#define D_LCD_PIN_D7   6
#define D_LCD_PIN_BL   7

#define D_LCD_PIN_E2   D_LCD_PIN_0

//Force I2C portexpander type
#define PCF8574        0
#define MCP23008       1

//Inverted Backlight control
#define BACKLIGHT_INV  0
#endif

#if (DFROBOT==1)
//Definitions for DFROBOT LCD2004 Module mapping between serial port expander pins and LCD controller
//This hardware uses PCF8574 and is different from earlier/different Arduino I2C LCD displays
//Slaveaddress hardwired to 0x4E. SDA/SCL has pullup Resistors onboard.
//See http://arduino-info.wikispaces.com/LCD-Blue-I2C
//
//Definitions for DFROBOT V1.1 
//This hardware uses PCF8574. Slaveaddress may be set by jumpers (default 0x40).
//SDA/SCL has pullup Resistors onboard and features a voltage level converter 3V3 <-> 5V.
//See http://www.dfrobot.com/index.php?route=product/product&product_id=135
//
//
//Note: LCD RW pin must be kept LOW
//      E2 is not available on default Arduino hardware and so it does not support LCD40x4 (second controller)
//      BL is used to control backlight
#define D_LCD_PIN_RS   0
#define D_LCD_PIN_RW   1
#define D_LCD_PIN_E    2
#define D_LCD_PIN_BL   3
#define D_LCD_PIN_D4   4
#define D_LCD_PIN_D5   5
#define D_LCD_PIN_D6   6
#define D_LCD_PIN_D7   7

#define D_LCD_PIN_E2   D_LCD_PIN_RW

//Force I2C portexpander type
#define PCF8574        1
#define MCP23008       0

//Inverted Backlight control
#define BACKLIGHT_INV  0
#endif

#if (YWROBOT==1)
//Definitions for YWROBOT LCM1602 V1 Module mapping between serial port expander pins and LCD controller. 
//Very similar to DFROBOT. This hardware uses PCF8574.
//Slaveaddress may be set by solderbridges (default 0x4E). SDA/SCL has no pullup Resistors onboard.
//See http://arduino-info.wikispaces.com/LCD-Blue-I2C
//
//Note: LCD RW pin must be kept LOW
//      E2 is not available on default hardware and so it does not support LCD40x4 (second controller)
//      BL is used to control backlight, reverse logic: Low turns on Backlight. This is handled in setBacklight()
#define D_LCD_PIN_RS   0
#define D_LCD_PIN_RW   1
#define D_LCD_PIN_E    2
#define D_LCD_PIN_BL   3
#define D_LCD_PIN_D4   4
#define D_LCD_PIN_D5   5
#define D_LCD_PIN_D6   6
#define D_LCD_PIN_D7   7

#define D_LCD_PIN_E2   D_LCD_PIN_RW

//Force I2C portexpander type
#define PCF8574        1
#define MCP23008       0

//Inverted Backlight control
#define BACKLIGHT_INV  0
#endif

#if (GYLCD==1)
//Definitions for Arduino-IIC-LCD GY-LCD-V1 Module mapping between serial port expander pins and LCD controller. 
//Very similar to DFROBOT. This hardware uses PCF8574.
//Slaveaddress may be set by solderbridges (default 0x4E). SDA/SCL has pullup Resistors onboard.
//See http://arduino-info.wikispaces.com/LCD-Blue-I2C
//
//Note: LCD RW pin must be kept LOW
//      E2 is not available on default hardware and so it does not support LCD40x4 (second controller)
//      BL is used to control backlight, reverse logic: Low turns on Backlight. This is handled in setBacklight()
#define D_LCD_PIN_D4   0
#define D_LCD_PIN_D5   1
#define D_LCD_PIN_D6   2
#define D_LCD_PIN_D7   3
#define D_LCD_PIN_EN   4
#define D_LCD_PIN_RW   5
#define D_LCD_PIN_RS   6
#define D_LCD_PIN_BL   7

#define D_LCD_PIN_E2   D_LCD_PIN_RW

//Force I2C portexpander type
#define PCF8574        1
#define MCP23008       0

//Force Inverted Backlight control
#define BACKLIGHT_INV  1
#endif

#if (SYDZ==1)
//Definitions for SYDZ Module mapping between serial port expander pins and LCD controller. 
//Very similar to DFROBOT. This hardware uses PCF8574A and uses inverted Backlight control
//Slaveaddress may be set by switches (default 0x40). SDA/SCL has pullup Resistors onboard.
//See ebay
//
//Note: LCD RW pin must be kept LOW
//      E2 is not available on default hardware and so it does not support LCD40x4 (second controller)
//      BL is used to control backlight, reverse logic: Low turns on Backlight. This is handled in setBacklight()
#define D_LCD_PIN_RS   0
#define D_LCD_PIN_RW   1
#define D_LCD_PIN_E    2
#define D_LCD_PIN_BL   3
#define D_LCD_PIN_D4   4
#define D_LCD_PIN_D5   5
#define D_LCD_PIN_D6   6
#define D_LCD_PIN_D7   7

#define D_LCD_PIN_E2   D_LCD_PIN_RW

//Force I2C portexpander type
#define PCF8574        1
#define MCP23008       0

//Force Inverted Backlight control
#define BACKLIGHT_INV  1
#endif

//Bitpattern Defines for I2C PCF8574/PCF8574A, MCP23008 and SPI 74595 Bus expanders
//
#define D_LCD_D4       (1<<D_LCD_PIN_D4)
#define D_LCD_D5       (1<<D_LCD_PIN_D5)
#define D_LCD_D6       (1<<D_LCD_PIN_D6)
#define D_LCD_D7       (1<<D_LCD_PIN_D7)
#define D_LCD_RS       (1<<D_LCD_PIN_RS)
#define D_LCD_E        (1<<D_LCD_PIN_E)
#define D_LCD_E2       (1<<D_LCD_PIN_E2)
#define D_LCD_BL       (1<<D_LCD_PIN_BL)
//#define D_LCD_RW       (1<<D_LCD_PIN_RW)

#define D_LCD_BUS_MSK  (D_LCD_D4 | D_LCD_D5 | D_LCD_D6 | D_LCD_D7)
#define D_LCD_BUS_DEF  0x00

/* PCF8574/PCF8574A I2C portexpander slave address */
#define PCF8574_SA0    0x40
#define PCF8574_SA1    0x42
#define PCF8574_SA2    0x44
#define PCF8574_SA3    0x46
#define PCF8574_SA4    0x48
#define PCF8574_SA5    0x4A
#define PCF8574_SA6    0x4C
#define PCF8574_SA7    0x4E

#define PCF8574A_SA0   0x70
#define PCF8574A_SA1   0x72
#define PCF8574A_SA2   0x74
#define PCF8574A_SA3   0x76
#define PCF8574A_SA4   0x78
#define PCF8574A_SA5   0x7A
#define PCF8574A_SA6   0x7C
#define PCF8574A_SA7   0x7E

/* MCP23008 I2C portexpander slave address */
#define MCP23008_SA0   0x40
#define MCP23008_SA1   0x42
#define MCP23008_SA2   0x44
#define MCP23008_SA3   0x46
#define MCP23008_SA4   0x48
#define MCP23008_SA5   0x4A
#define MCP23008_SA6   0x4C
#define MCP23008_SA7   0x4E

/* MCP23008 I2C portexpander internal registers */
#define IODIR          0x00
#define IPOL           0x01
#define GPINTEN        0x02
#define DEFVAL         0x03
#define INTCON         0x04
#define IOCON          0x05
#define GPPU           0x06
#define INTF           0x07
#define INTCAP         0x08
#define GPIO           0x09
#define OLAT           0x0A

/* ST7032i I2C slave address */
#define ST7032_SA      0x7C

/* ST7036i I2C slave address */
#define ST7036_SA0     0x78
#define ST7036_SA1     0x7A
#define ST7036_SA2     0x7C
#define ST7036_SA3     0x7E

/* PCF21XX I2C slave address */
#define PCF21XX_SA0    0x74
#define PCF21XX_SA1    0x76

/* AIP31068 I2C slave address */
#define AIP31068_SA    0x7C

/* SSD1803 I2C slave address */
#define SSD1803_SA0    0x78
#define SSD1803_SA1    0x7A

/* US2066/SSD1311 I2C slave address */
#define US2066_SA0     0x78
#define US2066_SA1     0x7A

/* AC780 I2C slave address */
#define AC780_SA0      0x78
#define AC780_SA1      0x7A
#define AC780_SA2      0x7C
#define AC780_SA3      0x7E

//Some native I2C controllers dont support ACK. Set define to '0' to allow code to proceed even without ACK
//#define LCD_I2C_ACK    0
#define LCD_I2C_ACK    1


// Contrast setting, 6 significant bits (only supported for controllers with extended features)
// Voltage Multiplier setting, 2 or 3 significant bits (only supported for controllers with extended features)
#define LCD_DEF_CONTRAST    0x20

//ST7032 EastRising ERC1602FS-4 display
//Contrast setting 6 significant bits
//Voltage Multiplier setting 3 significant bits
#define LCD_ST7032_CONTRAST 0x18
#define LCD_ST7032_RAB      0x04

//ST7036 EA DOGM1603 display
//Contrast setting 6 significant bits
//Voltage Multiplier setting 3 significant bits
#define LCD_ST7036_CONTRAST 0x28
#define LCD_ST7036_RAB      0x04

//SSD1803 EA DOGM204 display
//Contrast setting 6 significant bits
//Voltage Multiplier setting 3 significant bits
#define LCD_SSD1_CONTRAST   0x28
#define LCD_SSD1_RAB        0x06

//US2066/SSD1311 EastRising ER-OLEDM2002-4 display
//Contrast setting 8 significant bits, use 6 for compatibility
#define LCD_US20_CONTRAST   0x3F
//#define LCD_US20_CONTRAST   0x1F

//PCF2113, PCF2119 display
//Contrast setting 6 significant bits
//Voltage Multiplier setting 2 significant bits
#define LCD_PCF2_CONTRAST   0x20
#define LCD_PCF2_S12        0x02

//PT6314 VFD display
//Contrast setting 2 significant bits, use 6 for compatibility
#define LCD_PT63_CONTRAST   0x3F


#endif //MBED_TEXTLCDCONFIG_H