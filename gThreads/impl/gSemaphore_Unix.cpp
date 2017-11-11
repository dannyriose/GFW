#ifdef __gnu_linux__
#include <gThreads/gSemaphore.h>
#include "gSemaphorePrivate.h"
#include <pthread.h>
#include <semaphore.h>
using namespace gfw;

gSemaphore::gSemaphore(gu32 count){
    csem;
    sem_t *sem=new sem_t;

    sem_init(sem,0,count);
    o->handle=sem;

}

bool gSemaphore::wait(){
    csem;
    o->m_waiting=true;
    sem_wait((sem_t *)o->handle);
    o->m_waiting=false;

    return false;
}

gu32 gSemaphore::release(gu32 count){
    csem;
    int scount;

    for(int i=0;i<count;i++)
        sem_post((sem_t *)o->handle);
    sem_getvalue((sem_t *)o->handle,&scount);

    return (gu32)scount;
}

gSemaphore::~gSemaphore(){
    csem;
    sem_destroy((sem_t *)o->handle);
    sem_t *sem=(sem_t *)o->handle;
    delete sem;
    delete o;
}

#endif
