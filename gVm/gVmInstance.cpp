#include "gVm.h"

using namespace gfw;


gu32 gVmInstance::memAlloc(gu32 size)
{
    gu32 ret;

    if(dataMemory.vmAlloc(size,&ret))
        return ret;
    else
        return 0xffffffff;
}


void gVmInstance::memFree(void *mem)
{
    dataMemory.vmFree(mem);
}



gVmInstance &gVmInstance::operator =(const gVmInstance &other)
{
    dataMemory=other.dataMemory;
    programMemory=other.programMemory;
    stack=other.stack;
    sp=other.sp;
    pc=other.pc;
    fsp=other.fsp;
    procs=other.procs;
    return *this;
}
