#include "gSemaphore.h"
#include "impl/gSemaphorePrivate.h"
using namespace gfw;
bool gSemaphore::isWaiting() const{
    cs;
    return o->m_waiting;
}


