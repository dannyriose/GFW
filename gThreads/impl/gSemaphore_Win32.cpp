#include <gThreads/gSemaphore.h>
#include "gSemaphorePrivate.h"
#ifdef WIN32
#include "Windows.h"
#ifdef DEBUG
#include <iostream>
#endif
using namespace gfw;
gSemaphore::gSemaphore(gu32 count)
{
    csem;
    o->handle=CreateSemaphore(0,0,count,0);

}

gSemaphore::~gSemaphore()
{
    cs;
    CloseHandle(o->handle);
    delete o;
}

bool gSemaphore::wait()
{
    cs;
    o->m_waiting=true;
    return (WaitForSingleObject(o->handle,INFINITE)==WAIT_OBJECT_0);
}

gu32 gSemaphore::release(gu32 count)
{
    cs;
    LONG l;
    o->m_waiting=false;
    if(!ReleaseSemaphore(o->handle,count,&l))
    {
#ifdef DEBUG
        std::cout << "FUCKING ERROR " << GetLastError();
#endif

    }

    return l;
}



#endif


