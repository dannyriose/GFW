#ifndef GTIMERLIST_H
#define GTIMERLIST_H

#include <gTimers/gTimer.h>
#include <gAlignedLists/gAlignedPtrList.h>


namespace gfw
{
class gTimerList: public gAlignedPtrList<gTimer>
{
public:
    gTimerList()
    {

    }
    gTimerList(const gTimerList &other): gAlignedPtrList<gTimer>(other)
    {

    }
    ~gTimerList()
    {

    }

    virtual gTimer *search(const gString &_sid,gu32 *indexOut = 0) const
    {
        gTimer *v;
        for(gu32 i = 0; i < this->d->m_size; i++)
        {
            v = this->value(i);
            if(v->getTimerStringId() == _sid)
            {
                if(indexOut)
                {
                    *indexOut = i;
                }
                return v;
            }
        }
        return 0;
    }
    virtual gTimer *search(gs32 _iid, gu32 *indexOut = 0) const
    {
        gTimer *v;
        for(gu32 i = 0; i < this->d->m_size; i++)
        {
            v = this->value(i);
            if(v->getTimerIntegerId() == _iid)
            {
                if(indexOut)
                {
                    *indexOut = i;
                }
                return v;
            }
        }
        return 0;
    }
};
}
#endif // GTIMERLIST_H
