#ifndef IO_H
#define	IO_H

#include <mbed.h>
#include "DebounceIn.h"
#include "Pressed.h"

static const PinName SELECT = D11;
static const PinName LEFT = D12;
static const PinName RIGHT = D10;
static const PinName MENU = D13;

static const PinName BTN_LED = D2;
static const PinName CON_LED = D8;

class io {
    public:
        io();
        void init();
        bool pressLeft();
        bool pressSelect();
        bool pressRight();
        bool pressMenu();
        bool btnPressAll();

        bool readLeft();
        bool readSelect();
        bool readRight();
        bool readMenu();
        bool btnReadAll();

        void led(bool on);
        void connectionLed(char status);
    private:
        bool leftPressed;
        Timer timeConLed;
        bool readyConPwm;
        float conLedPwm;
        bool conIncreasing;

};


#endif
