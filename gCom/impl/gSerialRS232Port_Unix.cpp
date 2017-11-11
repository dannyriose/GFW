#include "gSerialRS232PortPrivate.h"
#ifdef __gnu_linux__
gs32 gSerialRS232PortPrivate::open(gu32 _baudrate, bool bParity, gs32 retries){
    return 0;
}
bool gSerialRS232PortPrivate::send(const char *data, gu32 size){
    return true;
}
void gSerialRS232PortPrivate::receiver(){

}
void gSerialRS232PortPrivate::close(){

}
#endif
