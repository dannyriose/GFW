#ifndef GTIMERLIST_H
#define GTIMERLIST_H

#include <gTimers/gTimer.h>
#include <gObjectEngineModel/gEngineModelList/gEngineModelList.h>


namespace gfw
{
class gTimerList: public gEngineModelList<gTimer>
{
public:
    gTimerList()
    {

    }
    gTimerList(const gTimerList &other): gEngineModelList<gTimer>(other)
    {

    }
    ~gTimerList()
    {

    }
};
}
#endif // GTIMERLIST_H
