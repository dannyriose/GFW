#ifndef GTIMERSCOMMON_H
#define GTIMERSCOMMON_H
#include <gfw_global.h>
namespace gfw{
    class gTimer;
    class gSystemTimer;
    class gVirtualTimer;
    //! The timer callback function for system timers
    typedef void (*ITimerCallBack)(gTimer *timer);
    //! The timer callback function for virtual timers

    class gTimerCallBack{
    protected:
        gTimer *m_caller;
    public:
        gTimerCallBack(){

        }
        virtual ~gTimerCallBack(){

        }
        virtual void timeOut() =0;
        gTimer *getTimer(){return m_caller;}
        void setTimer(gTimer *timer){
            m_caller=timer;
        }

    };
}
#endif // GTIMERSCOMMON_H
