#ifndef GWAIT_H
#define GWAIT_H
#include "gThreads_Global.h"
namespace gfw
{
    class GTHREADS_LIB gWait
    {
    private:
        void *d;
    public:
        enum GWAIT_CONSTANTS
        {
            GWAIT_NONE=0,
            GWAIT_INFINITE=0xffffffff

        };

        gWait();
        ~gWait();
        bool wait(bool bAutoReset=true,unsigned long milliseconds=GWAIT_INFINITE);
        void awake();
        void setForWait();
        bool isWaiting();
    };
}
#endif // GWAIT_H
