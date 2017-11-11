#ifndef GSERIALRS232PRIVATE_H
#define GSERIALRS232PRIVATE_H
#include <gCom/gSerialRS232.h>
using namespace gfw;
using namespace gfw::gcom;

struct gSerialRS232Private:public gSharedObject{
    //! The list of RS232 ports
    gList<gSerialRS232Port> ports;
    gSerialRS232Private(){

    }

    ~gSerialRS232Private(){

    }

    virtual void copy(const gSharedObject *other){
        const gSerialRS232Private *o = static_cast<const gSerialRS232Private *>(other);
        ports.copyFromOther(o->ports,false);
    }
};

G_SHAREDOBJECT_FACTORY(gSerialRS232PrivateFactory,gSerialRS232Private);

#endif // GSERIALRS232PRIVATE_H
