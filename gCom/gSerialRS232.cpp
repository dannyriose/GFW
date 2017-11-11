#include "impl/gSerialRS232Private.h"
#define ca GFW_PRIVATE_CAST(o,d,gSerialRS232Private)
gSerialRS232::gSerialRS232():gBaseShared(new gSerialRS232PrivateFactory){

}

gSerialRS232::gSerialRS232(const gSerialRS232 &other):gBaseShared(other,new gSerialRS232PrivateFactory){

}

gSerialRS232 &gSerialRS232::operator = (const gSerialRS232 &other){
    assign(other);
    return *this;
}

gSerialRS232Port *gSerialRS232::addPort(const gString &name, gu32 bufferSize){
    ca;
    if(!o){
        o = new gSerialRS232Private();
        d=o;
    }
    gSerialRS232Port *p = new gSerialRS232Port(name,bufferSize,this);
    o->ports.append(p);
    return p;
}

gu32 gSerialRS232::getPorts(){
    return 0;
}

gSerialRS232Port *gSerialRS232::getPort(gu32 index) const{
    if(!d)return 0;
    ca;


    return o->ports.pValue(index);
}

gu32 gSerialRS232::getPortCount() const{
    if(!d)return 0;
    ca;
    return o->ports.size();
}
