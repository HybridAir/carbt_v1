#ifndef QN8027_H
#define	QN8027_H

#include <mbed.h>

class QN8027 {
    public:
		QN8027();
		void setBit(char address, char bitmask, char value);
    private:

};

#endif
