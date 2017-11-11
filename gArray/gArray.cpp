#include "impl/gArrayPrivate.h"
using namespace gfw;
/*****************************gArray Functions***********************************/
//da caster
#define ca GFW_PRIVATE_CAST(o,d,gArrayPrivate)
#define co GFW_PRIVATE_CAST(oh,other.d,gArrayPrivate)
#define cn if(!d)return
#define acn assert(d)
#define cs gArrayPrivate *o=new gArrayPrivate

gArrayPrivate *acr(gSharedObject **target)
{
    gArrayPrivate *o=new gArrayPrivate();
    *target=o;
    return o;
}

gArray::gArray():gBaseShared(new gArrayPrivateFactory){

}

gArray::gArray(const gArray &other):gBaseShared(other,new gArrayPrivateFactory){

}
//macro to check duplicates
#define checkdup(v) if(o->bAvoidDuplicates)if(contains(v))return;
gArray::gArray(const void *initialData):gBaseShared(new gArrayPrivateFactory){
    d=new gArrayPrivate;
    insertAtEnd(initialData);
}

gArray::gArray(const void *DataBlock, gu32 dataSize, gu32 uSize):gBaseShared(new gArrayPrivateFactory)
{
    d=new gArrayPrivate;
    insertAtEnd(DataBlock,dataSize,uSize);
}

gArray::~gArray(){

}

//Implement alloc here
bool gArray::alloc(gu32 usize)
{
    ca;
    if(!o){ // check if private member has been initialized
        o=acr(&d);//create the private member
    }
    return o->alloc(usize);//allocate data
}

void gArray::clearArray(bool bClearUserData){
    cn;
    ca;
    if(bClearUserData){
        IClean func=o->func;
        o->func=0;//we only set the cleaning function to 0
        o->clear();
        o->func=func;
    }else{
        o->clear();
    }
}

/*****************SetData***********/
void gArray::setData(void *data, gu32 index, bool removeOld)
{
    acn;
    ca;
    o->setData(data,index,removeOld);
}

void gArray::insertAtEnd(const void *data)
{
    assert(data);
    gArrayPrivate *o;
    if(!d)
        o=acr(&d);
    else
        o=(gArrayPrivate *)d;

    checkdup(data);
    o->insertAtEnd(data);
}

void gArray::insertAtEnd(const gArray &other)
{

    co;
    assert(oh);
    gArrayPrivate *o;
    if(!d)
        o=acr(&d);
    else
        o=(gArrayPrivate *)d;

    o->insertAtEnd(oh);
}

void gArray::insertAtEnd(const void *dataBlock, gu32 blockSize, gu32 arrSize)
{
    assert(dataBlock);
    assert(blockSize);
    assert(arrSize);
    gArrayPrivate *o;
    if(!d)
        o=acr(&d);
    else
        o=(gArrayPrivate *)d;

    o->insertAtEnd(dataBlock,blockSize,arrSize);
}

void gArray::insertAtFirst(const void *data)
{
    assert(data);
    gArrayPrivate *o;
    if(!d)
        o=acr(&d);
    else
        o=(gArrayPrivate *)d;
    checkdup(data);
    o->insertAtFirst(data);
}

void gArray::insertAtFirst(const gArray &other)
{
    co;
    assert(oh);
    gArrayPrivate *o;
    if(!d)
        o=acr(&d);
    else
        o=(gArrayPrivate *)d;

    o->insertAtFirst(oh);
}

void gArray::insertAtFirst(const void *data, gu32 blockSize, gu32 arrSize)
{
    assert(data);
    assert(blockSize);
    assert(arrSize);
    gArrayPrivate *o;
    if(!d)
        o=acr(&d);
    else
        o=(gArrayPrivate *)d;

    o->insertAtFirst(data,blockSize,arrSize);
}

void gArray::insert(const void *data, gu32 pos)
{
    assert(data);
    gArrayPrivate *o;
    if(!d)
        o=acr(&d);
    else
        o=(gArrayPrivate *)d;
    checkdup(data);
    o->insert(data,pos);
}

void gArray::insert(const gArray &other, gu32 pos)
{
    co;
    assert(oh);
    gArrayPrivate *o;
    if(!d)
        o=acr(&d);
    else
        o=(gArrayPrivate *)d;
    o->insert(oh,pos);
}

void gArray::insert(const void *data, gu32 blockSize, gu32 arrSize, gu32 pos)
{
    assert(data);
    assert(blockSize);
    assert(arrSize);
    gArrayPrivate *o;
    if(!d)
        o=acr(&d);
    else
        o=(gArrayPrivate *)d;
    o->insert(data,blockSize,arrSize,pos);
}

gu32 gArray::size() const
{
    cn 0;
    ca;
    return o->size;
}
void *gArray::getData(gu32 index) const
{
    acn;
    ca;
    return o->getData(index);
}

const void *gArray::constData(gu32 index) const
{
    acn;
    ca;
    return o->constData(index);
}

void gArray::lock()
{
    cn;
    ca;
    o->bLocked=true;
}

void gArray::unlock()
{
    cn;
    ca;
    o->bLocked=false;
}
void *gArray::removeAt(void *element, gu32 blockSize, bool bFreeMemory){
    if(!d) return 0;
    gu32 index;
    if(contains(element,&index)){
        return removeAt(index,blockSize,bFreeMemory);
    }
    return 0;
}

void *gArray::removeAt(gu32 index, gu32 blockSize, bool bFreeMemory)
{
    if(!d) return 0;
    ca;
    return o->removeAt(index,blockSize,bFreeMemory);
}

void *gArray::removeAtEnd(gu32 blockSize, bool bFreeMemory)
{
   if(!d) return 0;
    ca;
    return o->removeAtEnd(blockSize,bFreeMemory);
}

void *gArray::removeAtFirst(gu32 blockSize, bool bFreeMemory)
{
   if(!d) return 0;
    ca;
    return o->removeAtFirst(blockSize,bFreeMemory);
}

void gArray::cleanFunction(IClean func)
{
    gArrayPrivate *o;
    if(!d){
        o=acr(&d);
    }else
        o=(gArrayPrivate *)d;

    o->func=func;
}
void gArray::allocFunction(IAlloc func){
    gArrayPrivate *o;
    if(!d){
        o=acr(&d);
    }else
        o=(gArrayPrivate *)d;

    o->afunc=func;
}
void gArray::setElementFactory(gArrayElementFactory *obj){
    gArrayPrivate *o;
    if(!d){
        o=acr(&d);
    }else
        o=(gArrayPrivate *)d;

    o->factory=obj;
}

bool gArray::isEmpty() const
{
    cn true;
    ca;
    return o->size==0;
}

bool gArray::isLocked() const
{
    cn false;
    ca;
    return o->bLocked;
}

void *gArray::operator [](gu32 index) const
{
    acn;
    ca;
    return o->getData(index);
}

gArray &gArray::operator =(const gArray &other){
    assign(other);
    return *this;
}

void gArray::resize(gu32 newSize, bool bFreeMemory)
{
    if(!d)
    {
        alloc(newSize);
    }
    else
    {
        ca;
        o->resize(newSize,bFreeMemory);
    }
}
bool gArray::contains(const void *element, gu32 *indexOut) const{
    if(!d)return false;
    ca;
    for(gu32 i=0;i<o->size;i++){
        if(o->data[i]==(a_ptr)element){
            if(indexOut)*indexOut=i;
            return true;
        }
    }
    return false;
}
bool gArray::search(const void *element, gu32 startPos, gu32 *indexOut, G_SEARCH_OPERATOR op) const{
    if(!d)return false;
    ca;
    bool ret;
    if(startPos >=o->size)return false;
    for(gu32 i=startPos;i<o->size;i++){
        G_OP_EQ(op,o->data[i],(a_ptr)element,ret);
        if(ret){
            if(indexOut)*indexOut=i;
            return true;
        }
    }
    return false;
}
gu32 gArray::count() const{
    if(!d)return 0;
    ca;
    return o->size;
}
gu32 gArray::count(const void *element, gUIntArray *outIndexes, G_SEARCH_OPERATOR op) const{
    if(!d)return 0;
    ca;
    gu32 c=0;
    bool bval=false;
    if(outIndexes)outIndexes->clear();
    for(gu32 i=0;i<o->size;i++){
        G_OP_EQ(op,o->data[i],(a_ptr)element,bval);
        if(bval){
            if(outIndexes)
                outIndexes->insert(i);
            c++;
        }
    }
    return c;
}
bool gArray::avoidDuplicates() const{
    cn false;
    ca;
    return o->bAvoidDuplicates;
}
void gArray::setAvoidDuplicates(bool bSet){
    gArrayPrivate *o;
    if(!d){
        o=acr(&d);
    }else
        o=(gArrayPrivate *)d;
    o->bAvoidDuplicates=bSet;
}
const guptr *gArray::arrayPointer() const{
    cn 0;
    ca;
    return o->data;
}
#ifdef GUSE_VIRTUALMEMORY
void gArray::setVirtualMemory(gVirtualMemory *vmObj){
    ca;
    if(!d){
        o=acr(&d);
    }

    o->vm=vmObj;
}
#endif
