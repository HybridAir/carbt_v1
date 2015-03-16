//so this is the library I'm assuming was provided by the maker of the qn8027 ic
//I'm currently trying to figure out how the fuck it's supposed to work
//and then make it work

#include "qndriver.h"

//the following two things are used to read and write from the device through i2c
//the following two are prototypes (I think) for two functions below
extern UINT8 QND_ReadReg(UINT8 adr);
//this comes from qnio.c, it seems to read a specific i2c register address, and then return it

extern UINT8 QND_WriteReg(UINT8 adr, UINT8 value);
//like the above one, but it writes data to an i2c address instead of reading


#define R_TXRX_MASK    0x20		//it's a bitmask used in QNF_SetRegBit for setting register bits
//not sure why it's needed yet, but it's different in other versions of this code

UINT8   qnd_Crystal = QND_CRYSTAL_DEFAULT;		//this is used to set the crystal type being used in the device, currently set to 0x2b
UINT8   qnd_PrevMode;					//it isn't used anywhere so I guess ignore it


UINT8   qnd_Country  = COUNTRY_CHINA ;		//used to set the frequency limits based on the country of operation (boring FCC stuff)
UINT16  qnd_CH_START = 7600;				//the beginning of the allowed channel range
UINT16  qnd_CH_STOP  = 10800;				//the ending
UINT8   qnd_CH_STEP  = 1;					//I'm assuming this is how many spaces in between a channel (like 88.7, 88.9, etc)
//the above three vars are already defined below somewhere, so I don't know why they're doing it here
//it needs to be set to USA settings




/**********************************************************************
void QNF_SetRegBit(UINT8 reg, UINT8 bitMask, UINT8 data_val)
**********************************************************************
Description: set register specified bit

Parameters:
    reg:        register that will be set
    bitMask:    mask specified bit of register
    data_val:    data will be set for specified bit
Return Value:
    None
**********************************************************************/
//ok so, this thing is used to set a bit within a specific register
void QNF_SetRegBit(UINT8 reg, UINT8 bitMask, UINT8 data_val)
{

    UINT8 temp;					//temporary internal variable

    temp = QND_ReadReg(reg);		//get register's current value

    temp &= (UINT8)(~bitMask);		//fucking
    //it looks like we are doing a bitwise AND here
    //so take the value we got from before, and AND it with the bitmask
    //this lets us only modify a specific bit (bits?) of that data we got

    temp |= data_val & bitMask;		//ugh why the fuck
    //ok so the ANDed value we got from before is now being ORed with the data_val and bitmask ANDed together

//    temp |= data_val;
    //I didn't comment this above thing out, so I guess they didn't like it

    QND_WriteReg(reg, temp);			//write that fixed data to the register we wanted finally
    //kill me

    //this needs an exmaple

}

/**********************************************************************
UINT16 QNF_GetCh()
**********************************************************************
Description: get current channel frequency

Parameters:
    None
Return Value:
    channel frequency
**********************************************************************/
//it does what it says, and returns some weird looking thing
UINT16 QNF_GetCh()
{

    UINT8 tCh;
    UINT8  tStep;
    UINT16 ch = 0;
    // set to reg: CH_STEP
    tStep = QND_ReadReg(CH_STEP);			//read out everything in the SYSTEM address
    tStep &= CH_CH;							//AND it with the current channel bitmask
    ch  =  tStep ;							//ch is now what we got above
    tCh= QND_ReadReg(CH);					//now we're reading out whatever is in the CH1 address
    ch = (ch<<8)+tCh;						//remember that thing we anded before? now we're fucking
    //shifiting it left 8 bits, and adding tCh to it

    return CHREG2FREQ(ch);		//we're finally done

    //EXAMPLE
    //CH_STEP = 00000000 //this is the register
    //CH_CH =  00000011 //this is the bitmask for the above register that we want
    //CH = 00000001 //another bitmask

    //tStep = read(CH_STEP) //lets say we get 00000011 out of it
    //tStep &= CH_CH = 00000011 & 00000011 = 11 //so we got the last two bits we wanted out
    //ch  =  tStep; ch = 11 //self explanatory
    //tCh= read(CH = 00000001); //this reads out of CH1, lets say it gives us 10101010
    //ch = (ch<<8)+tCh; //so we shift 11 8 bits left, then add 10101010 to it
    //so we get 1100000000, and then add 10101010, finally giving us 1110101010
    //so was the first stuff actually fucking necessary
    //whatever, it gets the current channel out from SYSETM and CH1, and should work

    //so all we are doing it getting the last two bits out of SYSTEM, and putting them in front of everything in CH1
    //giving us 10 bits

}

/**********************************************************************
UINT8 QNF_SetCh(UINT16 freq)
**********************************************************************
Description: set channel frequency

Parameters:
    freq:  channel frequency to be set
Return Value:
    1: success
**********************************************************************/
//sets the frequency, needs the frequency as a 2 byte int
UINT8 QNF_SetCh(UINT16 freq)
{

    // calculate ch parameter used for register setting //not me
    UINT8 tStep;
    UINT8 tCh;
    UINT16 f;
        f = FREQ2CHREG(freq);		//do math on the input frequency ((freq-7600)/5) found in the .h
        //i guess we are converting the frequency to something nicer?

        // set to reg: CH //not me
        tCh = (UINT8) f;		//cast the above result to an 8 bit int, cutting off everything in front
        QND_WriteReg(CH, tCh);	//write the data above to address CH1
        // set to reg: CH_STEP //not me

        //now we need to write out the top 2 bits to SYSTEM (since there are 10 total)
        tStep = QND_ReadReg(CH_STEP);		//read out all the data in the SYSTEM address
        tStep &= ~CH_CH;		//AND the data we got with the NOTed SYSTEM bitmask
        tStep |= ((UINT8) (f >> 8) & CH_CH);		//OR the above result with (the converted frequency above shifted right 8, ANDed with CH_CH)
        QND_WriteReg(CH_STEP, tStep);		//write the remaining clusterfuck to it's proper place

    return 1;			//return 1 for some dumb reason

    //EXAMPLE
    //lets say we want to change the channel to 99.1 mhz
    //convert it to 9910 = 10011010110110

    //f = 462
    //tCh = 206 //since 111001110 to UINT8 = 11001110
    //QND_WriteReg(0x01, 206);

    //tStep = lets say 00000010
    //tStep = 00000010 AND 11111100 (~00000011 aka 0x03 aka CH_CH) = 0
    //tStep = 0 OR 1 (1 AND 00000011 = 1, converted to UINT8) = 1
    //QND_WriteReg(0x00, 00000001);  //this is right

}

/**********************************************************************
void QNF_SetAudioMono(UINT8 modemask, UINT8 mode)
**********************************************************************
Description:    Set audio output to mono.

Parameters:
  modemask: mask register specified bit
  mode
        QND_RX_AUDIO_MONO:    RX audio to mono
        QND_RX_AUDIO_STEREO:  RX audio to stereo
        QND_TX_AUDIO_MONO:    TX audio to mono
        QND_TX_AUDIO_STEREO:  TX audio to stereo
Return Value:
  None
**********************************************************************/
void QNF_SetAudioMono(UINT8 modemask, UINT8 mode)
{

    QNF_SetRegBit(SYSTEM2,modemask, mode);
    //SYSTEM2 is the same address as SYSTEM1, I guess the original programmer was a little high
    //modemask should always be 00010000, that's the only bit that applies
    //mode can be either QND_TX_AUDIO_MONO or QND_TX_AUDIO_STEREO (0x10 or 0x00)
    //this one is easy

}

/**********************************************************************
int QND_Delay()
**********************************************************************
Description: Delay for some ms, to be customized according to user
             application platform

Parameters:
        ms: ms counts
Return Value:
        None

**********************************************************************/
//used for delaying?
//need to put my own code in here
void QND_Delay(UINT16 ms)
{

	wait_ms(ms);		//there we go

//    UINT16 i,k;
//    for(i=0; i<3000;i++)
//    {
//
//        for(k=0; k<ms; k++)
//        {
//
//
//
//}
//
//}

}

/**********************************************************************
void QN_ChipInitialization()
**********************************************************************
Description: chip first step initialization, called only by QND_Init()

Parameters:
    None
Return Value:
    None
**********************************************************************/
//this resets and recalibrates the device, don't call it by itself (use QND_Init())
void QN_ChipInitialization()
{

    // reset
    QND_WriteReg(0x00,0x80);		//tell the device to reset to all default values (SWRST 1)
    QND_Delay(20);					//delay 20 ms for some reason
    //to be customized: change the crystal setting, according to HW design	//not me
    //.........
    //recalibration
    QNF_SetRegBit(0x00,0x40,0x40);	//reset the FSM (RECAL 1)
    QNF_SetRegBit(0x00,0x40,0x00);	//done resetting the FSM (RECAL 0)
    QND_Delay(20);                //delay 20 ms

    //the following two are not even in the datasheet
    //leave the alone I guess
    QND_WriteReg(0x18,0xe4);        //reset //not me
    QND_WriteReg(0x1b,0xf0);
    //Done chip recalibration

}

/**********************************************************************
int QND_Init()
**********************************************************************
Description: Initialize device to make it ready to have all functionality ready for use.

Parameters:
    None
Return Value:
    1: Device is ready to use.
    0: Device is not ready to serve function.
**********************************************************************/
//initalizes the device
UINT8 QND_Init()
{

    QN_ChipInitialization();
//    QND_WriteReg(0x00,  0x01); //resume original status of chip /* 2008 06 13 */
    return 1;
//is that seriously it
}

/**********************************************************************
void QND_SetSysMode(UINT16 mode)
***********************************************************************
Description: Set device system mode(like: sleep ,wakeup etc)
Parameters:
    mode:  set the system mode , it will be set by  some macro define usually:

    SLEEP (added prefix: QND_MODE_, same as below):  set chip to sleep mode
    WAKEUP: wake up chip
    TX:     set chip work on TX mode
    RX:     set chip work on RX mode
    FM:     set chip work on FM mode
    AM:     set chip work on AM mode
    TX|FM:  set chip work on FM,TX mode
    RX|AM;  set chip work on AM,RX mode
    RX|FM:    set chip work on FM,RX mode
Return Value:
    None
**********************************************************************/
//ok so this is basically used to set IDLE mode
//either we're sleeping (IDLE) or we're awake (transmitting)
//we can only give it QND_MODE_SLEEP or QND_MODE_WAKEUP
void QND_SetSysMode(UINT16 mode)
{

    UINT8 val;
    switch(mode)		//switch depending on the input mode (0-2 it seems)
    {

    case QND_MODE_SLEEP:                       //set sleep mode //not me
        QNF_SetRegBit(SYSTEM1, R_TXRX_MASK, 0);
        //set the third bit in the SYSTEM address to 0, sets the device to IDLE mode (TXREQ 0)
        break;
    default:		//this default looks like garbage tbh
            val = (UINT8)(mode >> 8);
            if (val)
            {

                val = val >> 1;
                if (val & 0x20)
                {

                    // set to new mode if it's not same as old
                    if((QND_ReadReg(SYSTEM1) & R_TXRX_MASK) != val)
                    {

                        QNF_SetRegBit(SYSTEM1, R_TXRX_MASK, val);

}
                    // make sure it's working on analog output
                    QNF_SetRegBit(SYSTEM1, 0x08, 0x00);

}

}
        break;

}

}

/**********************************************************************
void QND_TuneToCH(UINT16 ch)
**********************************************************************
Description: Tune to the specific channel. call QND_SetSysMode() before
call this function
Parameters:
    ch
    Set the frequency (10kHz) to be tuned,
    eg: 101.30MHz will be set to 10130.
Return Value:
    None
**********************************************************************/
void QND_TuneToCH(UINT16 ch)
{

    QNF_SetCh(ch);

}

/**********************************************************************
void QND_SetCountry(UINT8 country)
***********************************************************************
Description: Set start, stop, step for RX and TX based on different
             country
Parameters:
country:
Set the chip used in specified country:
    CHINA:
    USA:
    JAPAN:
Return Value:
    None
**********************************************************************/
void QND_SetCountry(UINT8 country)
{

    qnd_Country = country;
    switch(country)
    {

    case COUNTRY_CHINA:
        qnd_CH_START = 7600;
        qnd_CH_STOP = 10800;
        qnd_CH_STEP = 1;
        break;
    case COUNTRY_USA:
        qnd_CH_START = 8810;
        qnd_CH_STOP = 10790;
        qnd_CH_STEP = 2;
        break;
    case COUNTRY_JAPAN:
        break;

}

}

/**********************************************************************
UINT8 QND_TXSetPower( UINT8 gain)
**********************************************************************
Description:    Sets FM transmit power attenuation.

Parameters:
    gain: The transmission power attenuation value, for example,
          setting the gain = 0x13, TX attenuation will be -6db
          look up table see below
BIT[5:4]
            00    0db
            01    -6db
            10    -12db
            11    -18db
BIT[3:0]    unit: db
            0000    124
            0001    122.5
            0010    121
            0011    119.5
            0100    118
            0101    116.5
            0110    115
            0111    113.5
            1000    112
            1001    110.5
            1010    109
            1011    107.5
            1100    106
            1101    104.5
            1110    103
            1111    101.5
for example:
  0x2f,    //111111    89.5
  0x2e,    //111110    91
  0x2d,    //111101    92.5
  0x2c,    //111100    94
  0x1f,    //111011 95.5
  0x1e,    //111010 97
  0x1d,    //111001 98.5
  0x1c,    //111000 100
  0x0f,    //001111    101.5
  0x0e,    //001110    103
  0x0d,    //001101    104.5
  0x0c,    //001100    106
  0x0b,    //001011    107.5
  0x0a,    //001010    109
  0x09,    //001001    110.5
  0x08,    //001000    112
  0x07,    //000111    113.5
  0x06,    //000110    115
  0x05,    //000101    116.5
  0x04,    //000100    118
  0x03,    //000011    119.5
  0x02,    //000010    121
  0x01,    //000001    122.5
  0x00     //000000    124
**********************************************************************/
void QND_TXSetPower( UINT8 gain)
{

    UINT8 value = 0;
    value |= 0x40;
    value |= gain;
    QND_WriteReg(PAG_CAL, value);

}

/**********************************************************************
void QND_TXConfigAudio(UINT8 optiontype, UINT8 option )
**********************************************************************
Description: Config the TX audio (eg: volume,mute,etc)
Parameters:
  optiontype:option :
    QND_CONFIG_AUTOAGC:‘option'set auto AGC, 0:disable auto AGC,1:enable auto AGC.
    QND_CONFIG_SOFTCLIP;‘option’set softclip,0:disable soft clip, 1:enable softclip.
    QND_CONFIG_MONO;‘option’set mono,0: QND_AUDIO_STEREO, 1: QND_AUDIO_STEREO
    QND_CONFIG_AGCGAIN;‘option’set AGC gain, range:0000~1111
Return Value:
  none
**********************************************************************/
void QND_TXConfigAudio(UINT8 optiontype, UINT8 option )
{

    switch(optiontype)
    {

    case QND_CONFIG_MONO:
        if (option)
            QNF_SetAudioMono(0x10, QND_TX_AUDIO_MONO);
        else
            QNF_SetAudioMono(0x10, QND_TX_AUDIO_STEREO);
        break;

    case QND_CONFIG_MUTE:
        if (option)
            QNF_SetRegBit(23, 0x30,0);
        else
            QNF_SetRegBit(23, 0x30,0x30);
        break;
    case QND_CONFIG_AGCGAIN:
        QNF_SetRegBit(0x04, 0x70,(UINT8)(option<<4));
        break;
    default:
        break;

}

}
