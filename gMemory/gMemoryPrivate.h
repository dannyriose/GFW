#ifndef GMEMORYPRIVATE_H
#define GMEMORYPRIVATE_H
#include "gMemory.h"
#include <stdlib.h>
#include <memory.h>
namespace gfw{
struct gMemoryPrivate:public gSharedObject
{
    gu8 *data;
    gu32 size;
    gu32 blocksize;
    gu32 totalSize;
    int type;
    bool bOwnsData;
    gVirtualMemory *vm;

    gMemoryPrivate():data(0),
        size(0),
        blocksize(0),
        totalSize(0),
        type(0),
        bOwnsData(true),
        vm(0)
    {

    }
    ~gMemoryPrivate(){
        clear();

    }
    bool alloc(gu32 blockSize,gu32 Size,int _type){
        clear();
        totalSize=Size * blockSize;
        switch(_type){
        case GMEM_STDALLOC:
            data=(gu8 *)malloc(totalSize);
            break;
        case GMEM_GVMALLOC:
            gassert(vm,"gVirtualMemory object not specified on gMemory");
            data=(gu8 *)vm->vmAlloc(totalSize);
            break;
        }
        if(data)
        {
            size=Size;
            type=_type;
            blocksize=blockSize;
            return true;
        }
        else
        {
            clear();
            return false;
        }
    }

    void clear(){
        if(!data)return;;
        if(bOwnsData){
            switch(type){
            case GMEM_STDALLOC:free(data);break;
            case GMEM_GVMALLOC:vm->vmFree(data);break;
            }
        }
        data=0;size=0;
        blocksize=0;totalSize=0;
        bOwnsData=true;
    }
    virtual void copy(const gSharedObject *other){
        GFW_PRIVATE_CAST(o,other,gMemoryPrivate);
        if(alloc(o->blocksize,o->size,o->type)){
            memcpy(data,o->data,totalSize);
        }
    }

};
G_SHAREDOBJECT_FACTORY(gMemoryPrivateFactory,gMemoryPrivate);
}
#endif // GMEMORYPRIVATE_H
