#include "gVirtualTimer.h"


using namespace gfw;


gVirtualTimer::gVirtualTimer():m_virtualTimeSpeed(1),
    m_realTimeStart(0){
    m_initialTimeStart=getRealTime();

}

gVirtualTimer::gVirtualTimer(const gVirtualTimer &other):gTimer(other),
    m_virtualTimeSpeed(other.m_virtualTimeSpeed),
    m_realTimeStart(other.m_realTimeStart),
    m_initialTimeStart(other.m_initialTimeStart){

}

gVirtualTimer::~gVirtualTimer(){

}

gVirtualTimer &gVirtualTimer::operator =(const gVirtualTimer &other){
    m_virtualTimeSpeed=other.m_virtualTimeSpeed;
    m_realTimeStart=other.m_realTimeStart;
    m_initialTimeStart=other.m_initialTimeStart;
    gTimer::operator =(other);
    return *this;
}
void gVirtualTimer::start(){
    m_realTimeStart=getRealTime();
    gTimer::start();
}
void gVirtualTimer::stop(){

    m_realTimeStart=0;
    gTimer::stop();
}

gu32 gVirtualTimer::getTime(){
    if(!m_running)
        return 0;
    return (gu32)(getRealTime() - m_realTimeStart) *m_virtualTimeSpeed;
}
void gVirtualTimer::poll(){
    if(!m_running)
        return;
    if(m_interval <= getTime()){
        timeOut();
        if(m_periodic)
            m_realTimeStart=getRealTime();
        else
            stop();
    }

}
gu32 gVirtualTimer::getElapsedVirtualTime(){
    return (gu32)(getRealTime() - m_initialTimeStart) * m_virtualTimeSpeed;
}
