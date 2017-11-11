#ifndef GTHREAD_H
#define GTHREAD_H
#include "gThreads_Global.h"
#include "gMutex.h"
#include "gWait.h"

namespace gfw
{
    typedef void (*ICallBack)(void *);
    //! Utility class that handles threads.
    class GTHREADS_LIB gThread
    {
    private:
        //! Private member
        void *d;
    public:
        //! Constructor
        gThread();
        //! Destructor
        ~gThread();
        //! Begins the execution of the thread
        void start(); //Start running
        //! Implement your thread function by subclassing gThread and reimplementing gThread::run()
        virtual void run();
        //!
        virtual void finished();//Thread finishes execution
        void stop();//stop thread
        void init(ICallBack notiFunc,ICallBack startFunc, ICallBack runFunc);

        bool wait(bool bAutoReset=true, unsigned long milliseconds=gWait::GWAIT_INFINITE);//Wait for a moment
        void wakeup();//Wake up7
        void setForWait();
        void setUserData(void *data);
        void sleep(unsigned long ms);
        void *h()const;
        bool isWaiting() const;
        bool threadStarted() const;
    };
}
#endif // GTHREAD_H
