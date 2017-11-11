#ifndef SEMAPHORE_DATA_H
#define SEMAPHORE_DATA_H
#include <gBase/gtypes.h>
namespace gfw{
struct gSemaphorePrivate{
    void *handle;
    bool m_waiting;
    gSemaphorePrivate():handle(0),m_waiting(false)
    {
    }
};
}

#define cs GFW_PRIVATE_CAST(o,d,gSemaphorePrivate)
#define csem GFW_PRIVATE_CREATE(o,d,gSemaphorePrivate)

#endif // SEMAPHORE_DATA_H
