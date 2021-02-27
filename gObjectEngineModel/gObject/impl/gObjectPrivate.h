#ifndef GOBJECTPRIVATE_H
#define GOBJECTPRIVATE_H

#include <gObjectEngineModel/gObject/gObject.h>
#include <gObjectEngineModel/gObject/gObjectList.h>
#include <gObjectEngineModel/gProperty/gProperty.h>
#include <gTimers/gVirtualTimer.h>
#include <gTimers/gSystemTimer.h>
#include <gTimers/gTimerList.h>
namespace gfw
{
class gObjectPrivate: public gSharedObject
{
public:
    gObjectPrivate()
    {

    }
    gObjectPrivate(const gObjectPrivate *other):gSharedObject(other)
    {

    }

    virtual void copy(const gSharedObject *other)
    {
        const gObjectPrivate *_other = static_cast<const gObjectPrivate *>(other);
        m_objects = _other->m_objects;
        m_timers = _other->m_timers;
        m_properties = _other->m_properties;
    }
public:
   gEngineModelList<gProperty> m_properties;
   gTimerList m_timers;
   gObjectList m_objects;
   gObject *m_parent;
};

G_SHAREDOBJECT_FACTORY(gObjectPrivateFactory,gObjectPrivate);
//! Shortcut to ensure fast coding
#define G_OBJECTPD GFW_PRIVATE_CAST(pd,d,gObjectPrivate)

}

#endif // GOBJECTPRIVATE_H
