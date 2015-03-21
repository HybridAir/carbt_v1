#ifndef IO_H
#define	IO_H

#include <mbed.h>
#include "DebounceIn.h"

#define SELECT D11
#define LEFT D10
#define RIGHT D12
#define MENU D13

class io {
    public:
        io();
        void init();
        bool readLeft();
        bool readSelect();
        bool readRight();
        bool readMenu();
        bool btnReadAll();
        void led(bool on);
    private:

};


#endif
