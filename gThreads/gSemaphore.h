#ifndef GSEMAPHORE_H
#define GSEMAPHORE_H
#include "gThreads_Global.h"
namespace gfw
{
    class GTHREADS_LIB gSemaphore
    {
        void *d;
    public:
        gSemaphore(gu32 count);
        ~gSemaphore();
        bool wait();
        gu32 release(gu32 count);
        bool isWaiting() const;
    };
}
#endif // GSEMAPHORE_H
