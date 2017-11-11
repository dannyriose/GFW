#ifndef GSERIALRS232PORTPRIVATE_H
#define GSERIALRS232PORTPRIVATE_H

#include <gCom/gSerialRS232.h>
#include <gThreads/gThread.h>
#ifdef WIN32
#include <windows.h>
#endif
using namespace gfw;
using namespace gfw::gcom;
class gSerialRS232PortPrivate;
class gSerialThread:public gThread{
    gSerialRS232PortPrivate *m_data;
public:
    gSerialThread(gSerialRS232PortPrivate *data):m_data(data){

    }
    ~gSerialThread(){

    }
    virtual void run();
};

struct gSerialRS232PortPrivate:public gSharedObject{
#ifdef WIN32
    //! Com port handle
    HANDLE hComHandle;
    OVERLAPPED ros;
    OVERLAPPED wos;
    DWORD rxErrors;
#endif
    //! Baud rate this port was opened
    gu32 baudRate;
    //! The name assigned to this port
    gString name;
    //! Friendly name of this port
    gString friendlyName;
    //! Vendor ID
    gs32 vendorID;
    //! Product ID
    gs32 productID;
    //! Buffer to keep incoming data
    gByteArray buffer;
    //! Parent or holder of this port
    gSerialRS232 *parent;
    //! Pointer to main object
    gSerialRS232Port *portobj;
    //! Bytes written to buffer
    gu32 bytesRead;
    //! Next position to write on buffer
    gu32 nextPosition;
    //! Port opened
    bool opened;
    //! Max retries
    gs32 maxRetries;
    //!The thread
    gSerialThread thread;
    //! Helpful mutex
    gMutex mutex;
    gSerialRS232PortPrivate():gSharedObject(GSHARED_OBJECT_ALWAYS_SHARED),
        parent(0),
        portobj(0),
        bytesRead(0),
        opened(false),
        maxRetries(1),
        thread(this){

    }

    gSerialRS232PortPrivate(const gString &_name,gu32 bufferSize, gSerialRS232Port *_portobj,gSerialRS232 *_parent):
        gSharedObject(GSHARED_OBJECT_ALWAYS_SHARED),
    #ifdef WIN32
        hComHandle(INVALID_HANDLE_VALUE),
        errors(0),
    #endif
        name(_name),
        parent(_parent),
        portobj(_portobj),
        bytesRead(0),
        nextPosition(0),
        opened(false),
        maxRetries(1),
        thread(this){
#ifdef WIN32
        //Avoid mismatch of string types for windows
        if(name.isEmpty() == false){
#ifdef UNICODE
            if(name.format() == gString::ASCII)
                name.thisToUnicode();
#else
            if(name.format() == gString::UTF)
                name.thisToAscii();
#endif
        }
#endif
        buffer.alloc(bufferSize);
    }
    ~gSerialRS232PortPrivate(){

    }

    virtual void copy(const gSharedObject *other){
        const gSerialRS232PortPrivate *p = static_cast<const gSerialRS232PortPrivate *>(other);
        if(!p)return;
        *this = *p;
    }

#ifdef WIN32
    void purgeRx();
#endif
    //! Opens the COM port
    gs32 open(gu32 _baudrate,bool bParity, gs32 retries);
    //! Sends a buffer through the port
    bool send(const char *data, gu32 size);
    //! The reciever loop
    void receiver();
    //! close the communications port
    void close();

};

G_SHAREDOBJECT_FACTORY(gSerialRS232PortPrivateFactory,gSerialRS232PortPrivate);

#endif // GSERIALRS232PORTPRIVATE_H
