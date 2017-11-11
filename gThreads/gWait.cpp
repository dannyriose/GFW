#include "gWait.h"
#ifdef WIN32
#include <Windows.h>
#endif
#ifdef __gnu_linux__
#include <pthread.h>
#endif
#include "impl/gWaitPrivate.h"
using namespace gfw;

gWait::gWait()
{
    GFW_PRIVATE_CREATE(o,d,gWaitPrivate);
#ifdef WIN32
    o->waitObject=CreateEvent(0,true,false,0);
#endif
#ifdef __gnu_linux__
    pthread_mutexattr_t attributes;
    pthread_condattr_t condattributes;
    pthread_mutexattr_init(&attributes);
    pthread_condattr_init(&condattributes);
    pthread_mutexattr_settype(&attributes, PTHREAD_MUTEX_NORMAL);
    pthread_mutex_init(&o->mutex,&attributes);

    pthread_cond_init(&o->waitObject,&condattributes);
#endif
}


bool gWait::wait(bool bAutoReset, unsigned long milliseconds)
{
    bool bRet;
    GFW_PRIVATE_CAST(o,d,gWaitPrivate);
    o->b_waiting=true;

#ifdef WIN32

    bRet=(WaitForSingleObject(o->waitObject,milliseconds)==WAIT_OBJECT_0);
#endif
#ifdef __gnu_linux__
    pthread_mutex_lock(&o->mutex);
    if(milliseconds==0xffffffff){
        pthread_cond_wait(&o->waitObject,&o->mutex);
    }else{
        timespec ts;
        ts.tv_nsec=milliseconds * 1000000;
        ts.tv_sec=milliseconds / 1000;
        pthread_cond_timedwait(&o->waitObject,&o->mutex,&ts);
    }
    pthread_mutex_unlock(&o->mutex);
#endif
    o->b_waiting=false;
    if(bAutoReset){
#ifdef WIN32
        ResetEvent(o->waitObject);
#endif
    }

    return bRet;

}

void gWait::awake()
{
    GFW_PRIVATE_CAST(o,d,gWaitPrivate);
#ifdef WIN32
    SetEvent(o->waitObject);
#endif
#ifdef __gnu_linux__
    pthread_cond_signal(&o->waitObject);
#endif
}

void gWait::setForWait()
{
    GFW_PRIVATE_CAST(o,d,gWaitPrivate);
#ifdef WIN32
    ResetEvent(o->waitObject);
#endif

}

bool gWait::isWaiting()
{
    GFW_PRIVATE_CAST(o,d,gWaitPrivate);
    return o->b_waiting;
}

gWait::~gWait()
{
    GFW_PRIVATE_CAST(o,d,gWaitPrivate);
#ifdef WIN32
    CloseHandle(o->waitObject);
#endif
#ifdef __gnu_linux__
    pthread_mutex_destroy(&o->mutex);
    pthread_cond_destroy(&o->waitObject);
#endif
    delete o;
}
