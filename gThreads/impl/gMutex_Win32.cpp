//Mutex implementation for windows
#ifdef _WIN32
#include <windows.h>
#include <gThreads/gMutex.h>
#include "gMutexPrivate.h"
#define cw GFW_PRIVATE_CAST(o,d,gMutexPrivate)
using namespace gfw;
gMutex::gMutex()
{
    gMutexPrivate *o=new gMutexPrivate;
    d=(void *)o;

    o->m_mutex=CreateMutex(0, FALSE, 0);
}


void gMutex::lock()
{
    cw;
    o->m_owner=GetCurrentThreadId();
    if(WaitForSingleObject(o->m_mutex,INFINITE)==WAIT_ABANDONED)
    {
        ReleaseMutex(o->m_mutex);
        WaitForSingleObject(o->m_mutex,0);
    }
    o->m_bLocked=true;
}

void gMutex::unlock()
{
    cw;
    ReleaseMutex(o->m_mutex);
    o->m_bLocked=false;
}

gMutex::~gMutex()
{
    cw;
    CloseHandle(o->m_mutex);
    delete o;
}
#endif
