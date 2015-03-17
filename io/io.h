#ifndef IO_H
#define	IO_H

#include <mbed.h>

#define SELECT D11
#define PREV D10
#define NEXT D12
#define MENU D13

class io {
    public:
        io();
        void doled();
    private:
        DigitalOut myled;

};


#endif
