#ifndef GMUTEX_H
#define GMUTEX_H
#include "gThreads_Global.h"
//A Generic mutex class

namespace gfw
{
    class GTHREADS_LIB gMutex
    {
        void *d;
    public:

        gMutex();
        ~gMutex();
        //classic functions
        void lock();
        void unlock();
        unsigned int getOwnerId();
        bool isLocked();
        void *h() const;
    };

}
#endif // GMUTEX_H
