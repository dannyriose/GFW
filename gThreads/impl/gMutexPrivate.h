#ifndef MUTEX_DATA_H
#define MUTEX_DATA_H

#endif // MUTEX_DATA_H
#include <gBase/gtypes.h>

namespace gfw{
    struct gMutexPrivate
    {
        gHandle m_mutex;//The mutex data
        bool m_bLocked;
        unsigned long m_owner;//Calling owner

        gMutexPrivate():m_mutex(0),m_bLocked(0),m_owner(0)
        {

        }
    };
}
