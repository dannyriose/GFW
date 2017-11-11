#include <gThreads/gMutex.h>
#include "gMutexPrivate.h"
#ifdef __gnu_linux__
#include <pthread.h>
#define cw GFW_PRIVATE_CAST(o,d,gMutexPrivate);
using namespace gfw;
gMutex::gMutex()
{
    gMutexPrivate *o=new gMutexPrivate;
    d=(void *)o;
    pthread_mutex_t *m_mutex=new pthread_mutex_t;

    pthread_mutexattr_t attributes;

    pthread_mutexattr_init(&attributes);
    pthread_mutexattr_settype(&attributes, PTHREAD_MUTEX_RECURSIVE);

    pthread_mutex_init(m_mutex, &attributes);
    o->m_mutex=m_mutex;
}


void gMutex::lock()
{
    cw;
    o->m_owner=pthread_self();
    pthread_mutex_lock((pthread_mutex_t *)o->m_mutex);
    o->m_bLocked=true;
}

void gMutex::unlock()
{
    cw;
    pthread_mutex_unlock((pthread_mutex_t *)o->m_mutex);
    o->m_bLocked=false;
}

gMutex::~gMutex()
{
    cw;
    pthread_mutex_t *m_mutex=(pthread_mutex_t *)o->m_mutex;
    pthread_mutex_destroy(m_mutex);
    pthread_mutex_t *mutex=(pthread_mutex_t *)o->m_mutex;
    delete mutex;
    delete o;
}

#endif


