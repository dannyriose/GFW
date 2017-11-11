//proclist implementation

#include "gVm.h"


using namespace gfw;


gVmProcList::gVmProcList()
{

}


gVmProcItem *gVmProcList::addProc(const gString &name, gu32 address)
{
    gVmProcItem *pi=new gVmProcItem;
    pi->procName=name;
    pi->procAddress=address;
    procs.append(pi);

    return pi;
}


gVmProcItem *gVmProcList::getProc(gu32 index)
{
    return procs.pValue(index);
}

gu32 gVmProcList::procCount() const
{
    return procs.size();
}

bool gVmProcList::isEmpty() const
{
    return procs.isEmpty();
}

void gVmProcList::alloc(gu32 size)
{
    procs.alloc(size);
}

void gVmProcList::setProc(gu32 index, gVmProcItem *item)
{
    procs.setValue(item,index);
}

gVmProcList &gVmProcList::operator =(const gVmProcList &other)
{
    procs.copyFromOther(other.list());
    return *this;
}
