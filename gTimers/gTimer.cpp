#include "gTimer.h"
#include <time.h>
#ifdef WIN32
#include <windows.h>
#elif __gnu_linux__
#include <sys/time.h>
#include <stdio.h>
#endif
#include "gSystemTimer.h"
#include "gVirtualTimer.h"
using namespace gfw;



gu32 gTimer::getRealTime() const{
#ifdef __gnu_linux__
    timeval tv;
    gettimeofday(&tv, 0);
    return (gu32)(tv.tv_sec * 1000) + (tv.tv_usec / 1000);
#elif WIN32
    return GetTickCount();
#endif
}

gu64 gTimer::getRealTimeus() const{
#ifdef __gnu_linux__
    timeval tv;
    gettimeofday(&tv, 0);
    return (gu64)(tv.tv_sec * 1000000) + (tv.tv_usec);
#elif WIN32
    return GetTickCount();
#endif
}



gTimer &gTimer::operator =(const gTimer &other){
    if(m_type!=other.m_type)
        return *this;
    m_sid=other.m_sid;
    m_iid=other.m_iid;
    m_callback=other.m_callback;
    m_callbackObject=other.m_callbackObject;
    m_interval=other.m_interval;

    return *this;
}
