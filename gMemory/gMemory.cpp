#include "gMemoryPrivate.h"
using namespace gfw;
#define cs d=new gMemoryPrivate
#define ca gMemoryPrivate *o=(gMemoryPrivate *)d
#define co gMemoryPrivate *io=(gMemoryPrivate *)other.d
gMemory::gMemory():gBaseShared(new gMemoryPrivateFactory)
{
}

gMemory::gMemory(bool bShared):gBaseShared(new gMemoryPrivateFactory)
{
    d=new gMemoryPrivate;
    setShared(bShared);
}

gMemory::gMemory(const gMemory &other):gBaseShared(other,new gMemoryPrivateFactory)
{

}

gMemory::gMemory(const void *data, gu32 blockSize, gu32 size,bool bCopy):gBaseShared(new gMemoryPrivateFactory)
{
    setData(data,blockSize,size,bCopy);
}


//Allocation

bool gMemory::alloc(gu32 blockSize, gu32 Size, int type)
{
   clear();
   GFW_PRIVATE_CREATE(o,d,gMemoryPrivate);
   return o->alloc(blockSize,Size,type);

}



void *gMemory::data() const
{
    if(!d) return 0;
    ca;
    return (void *) o->data;
}

void *gMemory::data(gu32 index) const
{
    if(!d) return 0;
    ca;
    if(!o->data) return 0;
    gu32 realIndex;
    realIndex=index * o->blocksize;
    assert(realIndex < o->totalSize);
    return (void *)&o->data[realIndex];
}

void *gMemory::offset(gu32 lOffset) const
{
    if(!d) return 0;
    ca;
    if(!o->data) return 0;
    assert(lOffset < o->totalSize);
    return (void *)&o->data[lOffset];
}

gu32 gMemory::size()const
{
    if(!d) return 0;
    ca;

    return o->size;
}

gu32 gMemory::blocksize() const
{
    if(!d) return 0;
    ca;

    return o->blocksize;
}

gu32 gMemory::sizeInBytes() const
{
    if(!d) return 0;
    ca;

    return o->totalSize;
}

void gMemory::setData(const void *data,
                      gu32 blockSize,
                      gu32 Size,
                      bool bCopy,
                      bool bTakeDataOwnership,
                      gs32 type)
{
    gMemoryPrivate *o;
    assert(data!=0);
    assert(blockSize!=0);
    assert(Size!=0);
    if(bCopy)
    {
        o=static_cast<gMemoryPrivate *>(d);
        if(!o){
            o=new gMemoryPrivate;d=o;
        }
        if(o->alloc(blockSize,Size,type)){
            memcpy(o->data,data,o->totalSize);
        }
    }
    else
    {
        clear();
        o=new gMemoryPrivate;
        d=o;

        o->data=(gu8 *)data;
        o->blocksize=blockSize;
        o->size=Size;
        o->type=type;
        o->bOwnsData=bTakeDataOwnership;
    }
}


void gMemory::setData(const gMemory &other, bool bCopy,int type)
{
    co;
    assert(io!=0);
    assert(io->data!=0);

    setData(io->data,io->blocksize,io->size,bCopy,io->bOwnsData,type);


}

void *gMemory::operator [](int index) const
{
    return data(index);
}

gMemory &gMemory::operator =(const gMemory &other)
{

    assign(other);
    return *this;
}
gMemory::operator const void *() const
{
    if(!d)return 0;
    ca;
    return o->data;
}


bool gMemory::isEmpty() const
{
    if(!d)return true;
    ca;
    return o->data==0;
}



gMemory::~gMemory()
{

}

void gMemory::memsetTo(gu8 value)
{
    if(!d) return;
    ca;
    if(!o->data)return;

    memset(o->data,(int)value,o->totalSize);
}

void gMemory::insertAtEnd(const void *data, gu32 Size,gu32 blockSize)
{
    gu8 *oldata;
    gu32 oldsize,totalSize;
    if(!d)
    {
        setData(data,blockSize,Size);
        return;
    }

    ca;
    oldata=o->data;
    oldsize=o->size;
    totalSize=o->totalSize;
    o->size+=Size;
    o->totalSize=o->size * o->blocksize;
    switch(o->type)
    {
    case GMEM_STDALLOC:
        o->data=(gu8 *)realloc(o->data,o->totalSize);
        break;
    case GMEM_GVMALLOC:
        gassert(o->vm,"gVirtualMemory object not set on gMemory");
        o->data=(gu8 *)o->vm->vmReAlloc(o->data,o->totalSize);
        break;
    }

    if(o->data)
    {
        memcpy(&o->data[totalSize],data,Size * o->blocksize);
    }
    else
    {
        o->data=oldata;
        o->size=oldsize;
        o->totalSize=totalSize;
    }

}


void gMemory::insertAtEnd(const gMemory &other)
{
    gu8 *oldata;
    gu32 oldsize,totalSize;
    if(!d)
    {
        setData(other);
        return;
    }

    ca;
    co;
    //make sure memory chunks are the same
    assert(o->blocksize==io->blocksize);
    assert(o->data!=0);
    assert(io->data!=0);
    oldata=o->data;
    oldsize=o->size;
    totalSize=o->totalSize;

    o->size+=io->size;
    o->totalSize=o->size * o->blocksize;
    switch(o->type)
    {
    case GMEM_STDALLOC:
        o->data=(gu8 *)realloc(o->data,o->totalSize);
        break;
    case GMEM_GVMALLOC:
        gassert(o->vm,"gVirtualMemory object not set on gMemory");
        o->data=(gu8 *)o->vm->vmReAlloc(o->data,o->totalSize);
        break;
    }

    if(o->data)
    {
        memcpy(&o->data[totalSize],io->data,io->totalSize);

    }
    else
    {
        o->data=oldata;
        o->size=oldsize;
        o->totalSize=totalSize;
    }
}

//Get a section on to a new memory object
gMemory gMemory::section(gu32 index, gu32 _size,bool newBlock) const
{
    if(!d)return gMemory();
    gMemory other;

    ca;
    co;

    if(!newBlock){
        other.setData(data(index),o->blocksize,_size,false,false);
    }
    else
        other.setData(data(index),o->blocksize,_size);

    other.setShared(true);

    return gMemory(other);
}


