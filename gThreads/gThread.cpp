#include "gThread.h"
#include "impl/gThreadPrivate.h"

using namespace gfw;
void gThread::run()
{
    ct;
    if(t->run)
        t->run(t->userdata);
}

void gThread::finished(){

}

void *gThread::h() const
{
    return d;
}
void gThread::init(ICallBack notiFunc, ICallBack startFunc,ICallBack runFunc)
{
    ct;

    t->run=runFunc;
    t->start=startFunc;
    t->finish=notiFunc;
}

void gThread::setUserData(void *data)
{
    ct;
    t->userdata=data;
}

bool gThread::isWaiting() const
{
    ct;
    return t->m_waiting;
}

bool gThread::threadStarted() const{
    ct;
    return t->m_threadStarted;
}
