#include "impl/gSerialRS232PortPrivate.h"


using namespace gfw;
using namespace gfw::gcom;
#define ca GFW_PRIVATE_CAST(o,d,gSerialRS232PortPrivate)
gSerialRS232Port::gSerialRS232Port():gBaseShared(new gSerialRS232PortPrivateFactory){
   d = new gSerialRS232PortPrivate(gString(),GSERIALPORT_DEFAULT_BUFFER_SIZE,this,0);
}

gSerialRS232Port::gSerialRS232Port(const gString &name, gu32 bufferSize, gSerialRS232 *parent):
    gBaseShared(new gSerialRS232PortPrivateFactory){
    d = new gSerialRS232PortPrivate(name,bufferSize,this,parent);
}

gSerialRS232Port::gSerialRS232Port(const gSerialRS232Port &other):gBaseShared(other,new gSerialRS232PortPrivateFactory){

}
gSerialRS232Port &gSerialRS232Port::operator = (const gSerialRS232Port &other){
    assign(other);
    return *this;
}

void gSerialRS232Port::clearBuffer(){
    if(!d)return;
    ca;
    o->buffer.setMemTo(0);
    o->bytesRead = 0;
}

gByteArray gSerialRS232Port::getBytesRead() const{
    if(!d)return gByteArray();
    ca;
    if(!o->bytesRead)return gByteArray();
    gByteArray ba;



    if(o->bytesRead == o->buffer.size()){
        ba=o->buffer;
    }else{
        ba=o->buffer.section(0,o->bytesRead,true);
    }
    ba.setShared(true);
    return ba;
}

gu32 gSerialRS232Port::getBytesReadCount() const{
    if(!d)return 0;
    ca;
    return o->bytesRead;
}

bool gSerialRS232Port::isOpened() const{
    if(!d)return false;
    ca;
    return o->opened;
}

//main tamales here
gs32 gSerialRS232Port::open(gu32 baudRate, bool bParity, gs32 retries){
    if(!d)return GSERIAL_PE_EMPTY;
    ca;
    return o->open(baudRate,bParity,retries);

}

void gSerialRS232Port::close(){
    if(!d)return;
    ca;
    o->close();
    o->bytesRead = 0;
    o->opened = false;

}




bool gSerialRS232Port::send(const gByteArray &array){
    if(!d)return false;
    ca;
    return o->send((const char *)array.data(),array.size());

}
bool gSerialRS232Port::send(const gString &value){
    if(!d)return false;
    ca;
    return o->send(value.ascii(),value.size());
}
// gSerialThread Implementation
void gSerialThread::run(){
    m_data->receiver();
}
