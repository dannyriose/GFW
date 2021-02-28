#include "impl/gObjectPrivate.h"

using namespace gfw;


gObject::gObject(gObject *_parent,gs32 _type):
    gSharedEngineModel(_type),
    gBaseShared(new gObjectPrivateFactory)
{
    gObjectPrivate *pd = new gObjectPrivate(this);
    d = pd;
    if(_parent)
    {
        _parent->addChild(this);
    }

}
gObject::~gObject()
{
    G_OBJECTPD;

    if(pd->m_parent)
    {
        pd->m_parent->drop();
    }
}

/**************************Star for Properties**************************************/
gProperty *gObject::addProperty(const gString _sid, const gVariant _data, const gString &_name, gs32 _iid)
{
    G_OBJECTPD;
    //not cool for copies
    gProperty *p;
    if(_sid.isEmpty() == false)
    {
        p= pd->m_properties.search(_sid);
        //not found continue else return 0
        if(p)
        {
            return 0;
        }
    }
    if(_iid != -1)
    {
        p = pd->m_properties.search(_iid);
        //again, if found we reject an ask for a diferent integer ID
        if(p)
        {
            return 0;
        }
    }
    //Set all data
    p = new gProperty();
    p->setStringID(_sid);
    p->setIntegerID(_iid);
    p->setData(_data);
    p->setName(_name);
    pd->m_properties.append(p);

    return p;

}
//addProperty for pointers
gProperty *gObject::addProperty(gProperty *_property)
{
    //We check there is no garbage on code
    gassert(_property == 0,"ERROR: gObject::addProperty(gProperty*) pointer gProperty must be  valid");
    G_OBJECTPD;
    gu32 _indexOut;
    gProperty *p;
    if(pd->m_properties.contains(_property, &_indexOut))
    {
        return 0;
    }

    const gString &_propertySID = _property->stringID();
    if(_propertySID.isEmpty() == false)
    {
        //now check if there is another property with same string ID
        p = pd->m_properties.search(_propertySID);
        //Property with same String id exists
        if(p)
        {
            //it exists then send 0
            return 0;
        }
    }
    gs32 _propertyIID = _property->integerID();
    if(_propertyIID != -1)
    {
        p = pd->m_properties.search(_propertyIID);
        if(p)
        {
            return 0;
        }
    }

    pd->m_properties.append(_property);

    return _property;

}
//*******************As for searching properties*****************************
gProperty *gObject::property(const gString &_sid, gu32 *indexOut) const
{
    G_OBJECTPD;
    return pd->m_properties.search(_sid,indexOut);
}
gProperty *gObject::property(gs32 _iid, gu32 *indexOut) const
{
    G_OBJECTPD;
    return pd->m_properties.search(_iid,indexOut);
}
gProperty *gObject::property(gu32 _index) const
{
    G_OBJECTPD;
    return pd->m_properties.value(_index);
}
//************************Getting the property count**************************/
gu32 gObject::propertyCount() const
{
    G_OBJECTPD;
    return pd->m_properties.size();
}
/***********************Now we set functions to remove properties from lists**/
bool gObject::removeProperty(gu32 _index)
{
    G_OBJECTPD;
    return pd->m_properties.remove(_index);
}
bool gObject::removeProperty(const gString &_sid)
{
    G_OBJECTPD;
    return pd->m_properties.remove(_sid);
}
bool gObject::removeProperty(gProperty *_property)
{
    G_OBJECTPD;
    return pd->m_properties.remove(_property);
}
bool gObject::removePropertyByIID(gs32 _iid)
{
    G_OBJECTPD;
    gu32 _indexOut;
    if(pd->m_properties.contains(_iid,&_indexOut))
    {
        return pd->m_properties.remove(_indexOut);
    }
    return false;
}
/******************************Start of Adding Timers**************************************/
gTimer *gObject::addTimer(const gString &_sid, gu32 _interval, gs32 _iid, gs32 _timerType)
{
    G_OBJECTPD;
    //check for duplicities
    gTimer *t;
    //duplicated string ID not allowed
    if(_sid.isEmpty() == false)
    {
        t = pd->m_timers.search(_sid);
        if(t)
        {
            return 0;
        }
    }
    //duplicated integer ID is not allowed
    if(_iid != -1)
    {
         t = pd->m_timers.search(_iid);
         if(t)
         {
             return 0;
         }
    }
    switch(_timerType)
    {
    case GTIMER_TYPE_VIRTUAL:
        t = new gVirtualTimer();
        break;
    case GTIMER_TYPE_SYSTEM:
        t = new gSystemTimer();
        break;
    default:
        //we assume a virtual timer always
        t = new gVirtualTimer();
        break;
    }
    t->setInterval(_interval);
    t->setStringID(_sid);
    t->setIntegerID(_iid);
    pd->m_timers.append(t);

    return t;

}

gTimer *gObject::addTimer(gTimer *_timer)
{
    G_OBJECTPD;
    gTimer *t;

    const gString &_sid = _timer->stringID();
    gs32 _iid= _timer->integerID();
     //verify duplicity
    //check for the pointer first
    if(pd->m_timers.contains(_timer))
    {
        return 0;
    }
    //check if there is a duplicated timer with same string ID
    if(_sid.isEmpty() == false)
    {
        t = pd->m_timers.search(_sid);
        if(t)
        {
            return 0;
        }
    }
    //check if we hace duplicates for a valid integer ID
    if(_iid != -1)
    {
        t = pd->m_timers.search(_iid);
        if(t)
        {
            return 0;
        }
    }

    pd->m_timers.append(_timer);
    return _timer;
}

/********Get timers from object***/
gTimer *gObject::timer(gu32 _index) const
{
    G_OBJECTPD;
    return pd->m_timers.value(_index);
}
gTimer *gObject::timer(const gString &_sid, gu32 *indexOut) const
{
    G_OBJECTPD;
    return pd->m_timers.search(_sid,indexOut);
}
gTimer *gObject::timer(gs32 _iid, gu32 *indexOut) const
{
    G_OBJECTPD;
    return pd->m_timers.search(_iid,indexOut);
}
/****Total Count of timers in object */
gu32 gObject::timerCount() const
{
    G_OBJECTPD;
    return pd->m_timers.size();
}
/******Removing Timers from gObject********/
bool gObject::removeTimer(gu32 _index)
{
    G_OBJECTPD;
    return pd->m_timers.remove(_index);
}
bool gObject::removeTimer(const gString &_sid)
{
    G_OBJECTPD;
    return pd->m_timers.remove(_sid);
}
bool gObject::removeTimer(gTimer *_timer)
{
    G_OBJECTPD;
    return pd->m_timers.remove(_timer);
}
bool gObject::removeTimerByIID(gs32 _iid)
{
    gu32 indexOut;
    G_OBJECTPD;

    if(pd->m_timers.contains(_iid,&indexOut))
    {
        return pd->m_timers.remove(indexOut);
    }
    return false;
}
/***************Handling Child Objects*****************/
/****Adding Child gObjects to object ******************/
bool gObject::addChild(gObject *_obj)
{
    //Same checkout for duplicity.As always blank strings and integer ids with -1 are
    //ignored and then added
    G_OBJECTPD;
    gObjectPrivate *o;
    const gString &_sid = _obj->stringID();
    gs32 _iid = _obj->integerID();

    //Getting private member of new child object.
    o = static_cast<gObjectPrivate *>(_obj->sharedObject());

    if(o->m_parent == this) //We are already there
    {
        return false;
    }
    if(pd->m_objects.contains(_obj))
    {
        return false;
    }
    if(_sid.isEmpty() == false)
    {
        if(pd->m_objects.contains(_sid))
        {
            return false;
        }
    }
    if(_iid != -1)
    {
        if(pd->m_objects.contains(_iid))
        {
            return false;
        }
    }

    if(o->m_parent)
    {   //remove reference of old parent
        o->m_parent->drop();
    }
    //We set the parent
    o->m_parent = this;
    //Increase reference count
    grab();
    //Adding Object
    pd->m_objects.append(_obj);
    return true;
}
//***************Getting child objects fro object***************/
gObject *gObject::child(gu32 _index) const
{
    G_OBJECTPD;
    return pd->m_objects.value(_index);
}
gObject *gObject::child(const gString &_sid, gu32 *indexOut) const
{
    G_OBJECTPD;
    return pd->m_objects.search(_sid,indexOut);
}
gObject *gObject::child(gs32 _iid, gu32 *indexOut) const{
    G_OBJECTPD;
    return pd->m_objects.search(_iid,indexOut);
}
//*****************Total Count of child objects ****************/
gu32 gObject::childCount() const
{
    G_OBJECTPD;
    return pd->m_objects.size();
}

//***************Removing child objects from object****************/
bool gObject::removeChild(gObject *_obj)
{
    G_OBJECTPD;
    gObjectPrivate *o = static_cast<gObjectPrivate *>(_obj->sharedObject());
    if(o->m_parent == this)
    {
        drop();
    }
    return pd->m_objects.remove(_obj);
}
bool gObject::removeChild(gu32 _index)
{
     G_OBJECTPD;
    gObject *_obj = pd->m_objects.value(_index);
    gObjectPrivate *o = static_cast<gObjectPrivate *>(_obj->sharedObject());
    if(o->m_parent == this)
    {
        drop();
    }
    return pd->m_objects.remove(_index);
}
bool gObject::removeChild(const gString &_sid)
{
    G_OBJECTPD;
    gu32 indexOut;
    gObject *_obj = pd->m_objects.search(_sid,&indexOut);
    if(!_obj)
    {
        return false;
    }
    gObjectPrivate *o = static_cast<gObjectPrivate *>(_obj->sharedObject());
    if(o->m_parent == this)
    {
        drop();
    }
    return pd->m_objects.remove(indexOut);
}
bool gObject::removeChildByIID(gs32 _iid)
{
    gu32 indexOut;
    G_OBJECTPD;
    gObject *_obj = pd->m_objects.search(_iid,&indexOut);
    if(!_obj)
    {
        return false;
    }
    gObjectPrivate *o = static_cast<gObjectPrivate *>(_obj->sharedObject());
    if(o->m_parent == this)
    {
        drop();
    }
    return pd->m_objects.remove(indexOut);
}
/********************Setting and getting the parent object*****/
bool gObject::setParent(gObject *_parent)
{
     return _parent->addChild(this);
}
gObject *gObject::parent() const
{
    G_OBJECTPD;
    return pd->m_parent;
}
/*******************Setting User Specific Functions **/
void gObject::init(const gVariantList &_params)
{
    gObject *o;

    G_OBJECTPD;
    for(gu32 i = 0; i < pd->m_objects.size(); i++)
    {
        o = pd->m_objects.value(i);
        o->init(_params);
    }

}
void gObject::update(const gVariantList &_params)
{
    gObject *o;
    gTimer *t;
    G_OBJECTPD;
    for(gu32 i = 0; i < pd->m_objects.size(); i++)
    {
        o = pd->m_objects.value(i);
        o->update(_params);
    }
    for(gu32 i = 0; i < pd->m_timers.size(); i++)
    {
        t = pd->m_timers.value(i);
        if(t->timerType() == GTIMER_TYPE_VIRTUAL)
        {
            t->poll();
        }

    }
}
void gObject::event(const gVariantList &_params)
{
    gObject *o;

    G_OBJECTPD;
    for(gu32 i = 0; i < pd->m_objects.size(); i++)
    {
        o = pd->m_objects.value(i);
        o->event(_params);
    }
}
void gObject::onTimer(gTimer *_caller)
{
    GFW_UNUSED(_caller);
}
