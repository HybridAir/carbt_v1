#ifndef IO_H
#define	IO_H

#include <mbed.h>
#include "DebounceIn.h"

#define SELECT D11
#define PREV D10
#define NEXT D12
#define MENU D13

class io {
    public:
        io();
        void init();
        bool btnCheckAll();
        void led(bool on);
    private:

};


#endif
