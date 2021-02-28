#ifndef GOBJECTTIMERCALLBACK_H
#define GOBJECTTIMERCALLBACK_H
#include <gTimers/gTimer.h>
#include <gObjectEngineModel/gObject/gObject.h>

namespace gfw
{

class gObjectTimerCallback: public gTimerCallBack
{
public:
    gObjectTimerCallback(gObject *_object = 0):
        m_object(_object)
    {

    }
    virtual void timeOut()
    {
        m_object->onTimer(m_caller);
    }
    void setObject(gObject *_object)
    {
        m_object = _object;
    }
private:
    gObject *m_object;
};

}



#endif // GOBJECTTIMERCALLBACK_H
