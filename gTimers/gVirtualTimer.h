#ifndef GVIRTUALTIMER_H
#define GVIRTUALTIMER_H
#include "gTimer.h"
namespace gfw{
    //! Implements a virtual timer.
    class SHARED_GFW gVirtualTimer:public gTimer{
    protected:
        //! The virtual time speed
        gf32 m_virtualTimeSpeed;
        //! Initial time for timer
        gu32 m_realTimeStart;
        //! Initial timer start
        gu32 m_initialTimeStart;
    public:
        gVirtualTimer();
        gVirtualTimer(const gVirtualTimer &other);
        ~gVirtualTimer();
        gVirtualTimer &operator = (const gVirtualTimer &other);
        virtual void start();
        virtual void stop();
        virtual gu32 getTime();
        virtual void poll();
        //! Returns the elapsed virtual time since timer was created
        gu32 getElapsedVirtualTime();
        //! Sets the timer speed
        void setTimerSpeed(gf32 speed){
            m_virtualTimeSpeed=speed;
        }
        gf32 timerSpeed() const{
            return m_virtualTimeSpeed;
        }

    };

}
#endif // GVIRTUALTIMER_H
