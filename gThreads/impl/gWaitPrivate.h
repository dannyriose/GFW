#ifndef GWAITPRIVATE_H
#define GWAITPRIVATE_H
#include <gThreads/gWait.h>
namespace gfw{
struct gWaitPrivate
{
    bool b_waiting;
#ifdef WIN32
    void *waitObject;
#endif
#ifdef __gnu_linux__
    pthread_cond_t waitObject;
    pthread_mutex_t mutex;
#endif

    gWaitPrivate():b_waiting(0)
    {
#ifdef __gnu_linux__

#endif
    }
};

}
#endif // GWAITPRIVATE_H
