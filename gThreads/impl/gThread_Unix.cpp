#ifdef __gnu_linux__
#include "gThreadPrivate.h"


//constructor

using namespace gfw;
#define ca GFW_PRIVATE_CAST(o,d,gThreadPrivate)

void *gThreadPrivate::threadFunction(void *data){
    gThread *gt=(gThread *)data;
    gThreadPrivate *td=(gThreadPrivate *)gt->h();

    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
    if(td->start)td->start(td->userdata);
    gt->run();
    if(td->finish)td->finish(td->userdata);
    gt->finished();
    td->m_threadStarted=false;
    return 0;
}

gThread::gThread():d(new gThreadPrivate){
}

void gThread::start(){
    ca;

    o->m_threadStarted=pthread_create(&o->threadID,NULL,&gThreadPrivate::threadFunction,this);
}
bool gThread::wait(bool bAutoReset, unsigned long milliseconds){
    GFW_UNUSED(bAutoReset);
    GFW_UNUSED(milliseconds);
    return false;
}

void gThread::setForWait(){

}

void gThread::wakeup(){

}

void gThread::stop(){
    ca;
    if(o->m_threadStarted)
        pthread_cancel(o->threadID);
}
gThread::~gThread(){
    ca;
    if(o->m_threadStarted)
        pthread_cancel(o->threadID);
    delete o;
}

#endif

