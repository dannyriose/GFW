#ifndef THREAD_DATA_H
#define THREAD_DATA_H
#include <gThreads/gThread.h>
#ifdef WIN32
#include <Windows.h>
#endif
#ifdef __gnu_linux__
#include <pthread.h>
#endif
namespace gfw{
struct gThreadPrivate
{
    void *handle;
    unsigned long threadID;
    ICallBack start;//Start function
    ICallBack finish;//Finish function
    ICallBack run;//Run func instead run virtual
    void *userdata;
    void *waitData;
    bool m_waiting;
    bool m_threadStarted;
    gThreadPrivate():handle(0),
        threadID(0),
        start(0),
        finish(0),
        run(0),
        userdata(0),
        waitData(0),
        m_waiting(false),
        m_threadStarted(false)
    {

    }
#ifdef _WIN32
    static DWORD __stdcall threadFunction(void *data);
#endif
#ifdef __gnu_linux__
    static void *threadFunction(void *data);
#endif
};
}
#define ct GFW_PRIVATE_CAST(t,d,gThreadPrivate)

#endif // THREAD_DATA_H
