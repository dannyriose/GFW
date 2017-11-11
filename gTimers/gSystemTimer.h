#ifndef GSYSTEMTIMER_H
#define GSYSTEMTIMER_H

#include "gTimer.h"
namespace gfw{
    //! Implementation of timer that runs using system timers.
    class SHARED_GFW gSystemTimer:public gTimer{
    protected:
        //! Private member.
        gHandle d;
    public:
        gSystemTimer();
        gSystemTimer(const gSystemTimer &other);
        ~gSystemTimer();
        gSystemTimer &operator = (const gSystemTimer &other);


        virtual void start();
        virtual void stop();
        virtual gu32 getTime();
        gHandle handle(){
            return d;
        }

    };
}
#endif // GSYSTEMTIMER_H
