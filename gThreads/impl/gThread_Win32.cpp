#ifdef _WIN32
#include "gThreadPrivate.h"
using namespace gfw;
//here is where the fun happens
DWORD gThreadPrivate::threadFunction(void *data)
{
    gThread *gt=(gThread *)data;
    gThreadPrivate *t=(gThreadPrivate *)gt->h();

    if(t->start)
        t->start(t->userdata);

    gt->run();

    if(t->finish)
        t->finish(t->userdata);
    gt->finished();
    return true;
}


gThread::gThread()
{
    GFW_PRIVATE_CREATE(t,d,gThreadPrivate);
    DWORD id;
    t->handle= CreateThread(
                NULL,       // default security attributes
                0,          // default stack size
                (LPTHREAD_START_ROUTINE)gThreadPrivate::threadFunction,
                this,       // no thread function arguments
                CREATE_SUSPENDED,// default creation flags
                &id); // receive thread identifier
    t->threadID=id;
    t->waitData=CreateEvent(0,TRUE,FALSE,0);


}

gThread::~gThread()
{
    ct;
    CloseHandle(t->handle);
    CloseHandle(t->waitData);
    delete t;
}

void gThread::start()
{
    ct;
    t->m_threadStarted=true;
    ResumeThread(t->handle);
}

void gThread::stop()
{
    ct;
    t->m_threadStarted=false;
    SuspendThread(t->handle);
}

bool gThread::wait(bool bAutoReset,unsigned long milliseconds)
{
    ct;
    bool bRet;

    t->m_waiting=true;
    bRet=WaitForSingleObject(t->waitData,milliseconds)==WAIT_OBJECT_0;
    t->m_waiting=false;
    if(bAutoReset)
        ResetEvent(t->waitData);

    return bRet;
}

void gThread::wakeup()
{
    ct;
    SetEvent(t->waitData);
}


void gThread::setForWait()
{
    ct;
    ResetEvent(t->waitData);
}

void gThread::sleep(unsigned long ms)
{
    Sleep(ms);
}
#endif
