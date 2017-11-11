#include "gSystemTimer.h"
#include <time.h>
#ifdef __gnu_linux__
#include <signal.h>
#include <unistd.h>
#define CLOCKID CLOCK_REALTIME
#define SIG SIGRTMIN
#elif WIN32
#include <windows.h>
#endif

#include <stdlib.h>
#include <stdio.h>


using namespace gfw;


struct gSystemTimerPrivate{
    static gList<gSystemTimer> _timers;
    static void addTimer(gSystemTimer *timer){
        _timers.append(timer);;
    }

    static void removeTimer(gSystemTimer *timer){
        _timers.remove(timer,1);
    }

    gu32 timestart;
#ifdef __gnu_linux__
    timer_t timerid;
    sigevent sev;
    itimerspec its;

    struct sigaction sa;
    sigset_t mask;
#endif
    template <typename T>
    static gSystemTimer *getTimer(T timerid){
        gSystemTimer *t;
        gSystemTimerPrivate *p;
        for(gu32 i=0;i<_timers.size();i++){
            t=_timers.pValue(i);
            p=(gSystemTimerPrivate *)t->handle();
            if(p->timerid == timerid)
                return t;
        }
        return 0;
    }
#ifdef __gnu_linux__
    static void timerHandler(int sig, siginfo_t *si,void *uc){
        timer_t *id=(timer_t *)si->si_value.sival_ptr;
        gSystemTimer *t=gSystemTimerPrivate::getTimer<timer_t>(*id);
        if(t)
            t->timeOut();
        signal(sig,SIG_IGN);

    }
#elif WIN32
    UINT_PTR timerid;
    static VOID CALLBACK timerHandler(HWND hwnd,UINT umsg, UINT_PTR idEvent,DWORD dwTime){
        gSystemTimer *t=gSystemTimerPrivate::getTimer<UINT_PTR>(idEvent);
        if(t)
            t->timeOut();
    }

#endif

    gSystemTimerPrivate(){
#ifdef __gnu_linux__
        sa.sa_flags=SA_SIGINFO;
        sa.sa_sigaction=gSystemTimerPrivate::timerHandler;

        sigemptyset(&sa.sa_mask);
        sigaction(SIG,&sa,0);
        sigemptyset(&mask);
        sigaddset(&mask,SIG);
        sigprocmask(SIG_SETMASK,&mask,NULL);

        sev.sigev_notify=SIGEV_SIGNAL;
        sev.sigev_signo=SIG;
        sev.sigev_value.sival_ptr=&timerid;
        timer_create(CLOCKID,&sev,&timerid);
#elif WIN32
        timerid=0;
#endif
    }
    gSystemTimerPrivate(const gSystemTimerPrivate &other):timerid(other.timerid)
  #ifdef __gnu_linux__
        ,sev(other.sev),
        its(other.its),
        sa(other.sa),
        mask(other.mask)
  #endif
    {

    }
    gSystemTimerPrivate &operator=(const gSystemTimerPrivate &other){

        timerid=other.timerid;
#ifdef __gnu_linux__
        sev=other.sev;
        its=other.its;
        sa=other.sa;
        mask=other.mask;
#endif
        return *this;
    }
    ~gSystemTimerPrivate(){
#ifdef __gnu_linux__
        timer_delete(timerid);
#elif WIN32
        if(timerid)
            KillTimer(0,timerid);
#endif
    }
};
gList<gSystemTimer> gSystemTimerPrivate::_timers((IClean)0);
#define ca gSystemTimerPrivate *o=(gSystemTimerPrivate *)d

gSystemTimer::gSystemTimer():d(new gSystemTimerPrivate){

}
gSystemTimer::gSystemTimer(const gSystemTimer &other):gTimer(other){
    if(!other.d)return;
    d=new gSystemTimerPrivate(*(gSystemTimerPrivate *)other.d);
    gSystemTimerPrivate::addTimer(this);
}
gSystemTimer::~gSystemTimer(){
    ca;
    gSystemTimerPrivate::removeTimer(this);
    delete o;
}
gSystemTimer &gSystemTimer::operator =(const gSystemTimer &other){
    ca;
    gSystemTimerPrivate *io=(gSystemTimerPrivate *)other.d;
    *o=*io;
    gTimer::operator =(other);
    return *this;
}

void gSystemTimer::start(){
    ca;
    o->timestart=getRealTime();
#ifdef __gnu_linux__
    o->its.it_value.tv_sec=(time_t)m_interval * 0.001;
    o->its.it_value.tv_nsec=(m_interval * 1000000) % 1000000000;
    o->its.it_interval.tv_sec=o->its.it_value.tv_sec;;
    o->its.it_interval.tv_nsec=o->its.it_value.tv_nsec;
    timer_settime(o->timerid,0,&o->its,NULL);
#elif WIN32
    o->timerid=SetTimer(NULL,0,m_interval,(TIMERPROC)gSystemTimerPrivate::timerHandler);
#endif
}

void gSystemTimer::stop(){
    ca;
#ifdef __gnu_linux__
    o->its.it_value.tv_sec=0;
    o->its.it_value.tv_nsec=0;
    o->its.it_interval.tv_sec=o->its.it_value.tv_sec;;
    o->its.it_interval.tv_nsec=o->its.it_value.tv_nsec;
    timer_settime(o->timerid,0,&o->its,NULL);
#elif WIN32
    if(o->timerid)
        KillTimer(0,o->timerid);
    o->timerid=0;
#endif
}

gu32 gSystemTimer::getTime(){
    ca;
#ifdef __gnu_linux__
    itimerspec its;
    timer_gettime(o->timerid,&its);
    gu32 ms=m_interval - (gu32)(its.it_value.tv_sec * 1000 + its.it_value.tv_nsec * 0.000001);
    return ms;
#elif WIN32
    gu32 elapsed=getRealTime() - o->timestart;
    if(elapsed > m_interval)
        return m_interval;
    else
        return elapsed;
#endif

}
