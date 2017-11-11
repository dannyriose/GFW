#include "gMutex.h"
#include "impl/gMutexPrivate.h"

using namespace gfw;
bool gMutex::isLocked(){
    GFW_PRIVATE_CAST(o,d,gMutexPrivate);
    return o->m_bLocked;
}

gu32 gMutex::getOwnerId(){
   GFW_PRIVATE_CAST(o,d,gMutexPrivate);
   return o->m_owner;
}
