#ifndef GSERIALRS232_H
#define GSERIALRS232_H
#include <gfw_global.h>
#include "gSerialRS232Port.h"
namespace gfw{
namespace gcom{
    //! Utility class to handle the RS232 serial port
    class gSerialRS232:public gBaseShared{
    public:
        //! Class constructor
        gSerialRS232();
        //! Class copy constructor
        gSerialRS232(const gSerialRS232 &other);
        //! Operator assign
        gSerialRS232 &operator = (const gSerialRS232 &other);
        //! Adds manually a port
        gSerialRS232Port *addPort(const gString &name, gu32 bufferSize);
        //! Get the total count of ports from the computer.
        gu32 getPorts();
        //! A port receivs data from serial
        virtual void onReceive(gSerialRS232Port *port);
        //! Gets a port given an index
        gSerialRS232Port *getPort(gu32 index) const;
        //! Get total port count
        gu32 getPortCount() const;


    };
}
}
#endif // GSERIALRS232_H
