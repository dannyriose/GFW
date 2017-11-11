#include "gArrayPrivate.h"

///Implementation of gArray object
//Ba is the base object performing all operations

using namespace gfw;


//utility macros to make faster the coding of this class
#define lo if(bLocked) return
#define e if(!data) return
#define ie if(index>=size) return
#define ac(obj) (a_ptr)obj
//implement ba and ga at the same time to end faster
gArrayPrivate::gArrayPrivate():size(0),
    blocksize(0),
    data(0),
    bLocked(false),
    func(0),
    afunc(0),
    factory(0),
    bAvoidDuplicates(false)
  #ifdef GUSE_VIRTUALMEMORY
  ,vm(0)
  #endif
{
}
gArrayPrivate::~gArrayPrivate()
{
    clear();
}
/*****************************************Alloc*********************************/
bool gArrayPrivate::alloc(gu32 usize, gu32 ublocksize)
{
    lo false;//returns false when locked
    clear();//clears array between allocating again
    //we use malloc to create the array. The array is just a pointer of integers
#ifdef GUSE_VIRTUALMEMORY
    if(vm)
        data=(a_ptr *)vm->vmAlloc(usize * sizeof(a_ptr));
    else
#endif
    data=(a_ptr *)malloc(usize*sizeof(a_ptr));
    if(data)
    {
        memset(data,0,usize*sizeof(a_ptr));//Initial values set as 0
        blocksize=ublocksize;
        size=usize;
    }
    return data!=NULL;
}
void gArrayPrivate::elremove(a_ptr element){
    if(factory)
        factory->remove((void *)element);
    else if(func)
        func((void *)element);
}

/************************************************Clear***************************************/
//Clears the array data
void gArrayPrivate::clear(bool bRemove)
{
    lo;
    gu32 i;
    if(data){
        if(func || factory){
            for(i=0;i<size;i++){
                /* The clean function is called per each array element
                  is up to the client of this object to implement the cleaning function for their objects
                  */
                if(bRemove){
                   elremove(data[i]);
                }
            }
        }
#ifdef GUSE_VIRTUALMEMORY
        if(vm)
            vm->vmfree(data);//Free memory used by the array
        else
#endif
        free(data);
        //Reset all members to 0
        size=0;
        blocksize=0;
        data=0;
        func=0;
    }
}

/*********************************setData**************************************/
void gArrayPrivate::setData(void *pdata, gu32 index, bool removeOld)
{
    lo;//verify if not locked
    e;//check if data has been allocated
    ie;//check if index is not bigger than size


    if(removeOld && hascleaning() && data[index]){
        elremove(data[index]);//call cleaning funciton if replaced
    }
    data[index]=(a_ptr)pdata;//set new data
}

/**********************************insertAtEnd**************************************/
void gArrayPrivate::insertAtEnd(const void *pData)
{
    lo;//locked
    gu32 oldsize;
    a_ptr *oldpointer;
    if(!pData)return;
    oldsize=size;//keep the old size
    oldpointer=data;//keep old data pointer in case it fails

    size++;//we are adding a new value so increase the size by one
    //use realloc to increase the size of the array
 #ifdef GUSE_VIRTUALMEMORY
    if(vm)
        data=(a_ptr *)vm->vmReAlloc(data,size *sizeof(a_ptr));
    else
 #endif
        data=(a_ptr *)realloc(data,size*sizeof(a_ptr));
    if(data){
        //since old size was the total size, and as we increased size +1 value now oldsize contains size - 1
        //we set our data to newly allocated element in our array
        data[oldsize]=ac(pData);
    }
    else{ //If realloc fails we keep our all data..this has never happened
        data=oldpointer;
        size=oldsize;
    }
}

//*******************************insertAtEnd other***************************************
void gArrayPrivate::insertAtEnd(const gArrayPrivate *other)
{
    lo;//locked
    gu32 oldsize;
    a_ptr *oldpointer;
    //follows the same procedure but we are copying another array
    if(!other->data)return;
    oldsize=size;
    oldpointer=data;

    size+=other->size;//now increase size + the other array size
#ifdef GUSE_VIRTUALMEMORY
   if(vm)
       data=(a_ptr *)vm->vmReAlloc(data,size *sizeof(a_ptr));
   else
#endif
    data=(a_ptr *)realloc(data,size*sizeof(a_ptr));
    if(data)
    {
        //copy the array starting from size - 1 which is the start point of the new array
        memcpy(&data[oldsize],other->data,other->size*sizeof(a_ptr));
    }
    else
    {
        data=oldpointer;
        size=oldsize;
    }
}

//***********************************insertAtEnd Primitive Array*****************************/
void gArrayPrivate::insertAtEnd(const void *pData, gu32 blocksize,gu32 arrSize)
{
    lo;
    gu32 oldsize;
    a_ptr *oldpointer;
    const unsigned char *c;//use the smallest data type to represent the other array
    gu32 i;

    if(!pData)return;
    oldsize=size;
    oldpointer=data;
    //self explanatory here
    size+=arrSize;
#ifdef GUSE_VIRTUALMEMORY
   if(vm)
       data=(a_ptr *)vm->vmReAlloc(data,size *sizeof(a_ptr));
   else
#endif
    data=(a_ptr *)realloc(data,size*sizeof(a_ptr));
    if(data)
    {
        c=(const unsigned char *)pData;//cast the array to a byte pointer
        //here is the tricky part of how we copy the array
        for(i=0;i<arrSize;i++)
        {
            //ac is just a macro to cast any pointer to a_ptr
            /*start copying from oldsize which is the first element where the new array will be copied
            increase only by i per cycle.
            NOTICE we are only storing pointer addresses. */
            data[oldsize+i]=ac(&c[i*blocksize]);
        }
    }
    else
    {
        data=oldpointer;
        size=oldsize;
    }

}

/***************InsertAt first now**************************/
void gArrayPrivate::insertAtFirst(const void *pData)
{
    lo;
    gu32 oldsize;
    a_ptr *oldpointer;
    gArrayPrivate copy;
    if(!pData)return;

    if(!data)
    {
        insertAtEnd(pData);
        return;
    }
    oldsize=size;
    oldpointer=data;
    copy.insertAtEnd(this);//make a copy of this array
    size++;
    //same procedure as in insertAtEnd realloc memory
#ifdef GUSE_VIRTUALMEMORY
   if(vm)
       data=(a_ptr *)vm->vmReAlloc(data,size *sizeof(a_ptr));
   else
#endif
    data=(a_ptr *)realloc(data,size*sizeof(a_ptr));
    if(data)
    {
        //we set our data here
        data[0]=ac(pData);
        //move the array starting from position 1
        memcpy(&data[1],copy.data,oldsize*sizeof(a_ptr));
    }
    else
    {
        data=oldpointer;
        size=oldsize;
    }
}

void gArrayPrivate::insertAtFirst(const gArrayPrivate *other)
{
    lo;
    gu32 oldsize;
    a_ptr *oldpointer;
    gArrayPrivate copy;
    if(!other->data)
        return;

    if(!data)
    {
        insertAtEnd(other);
        return;
    }
    oldsize=size;
    oldpointer=data;

    copy.insertAtEnd(this);//make a copy
    size+=other->size;
#ifdef GUSE_VIRTUALMEMORY
   if(vm)
       data=(a_ptr *)vm->vmReAlloc(data,size *sizeof(a_ptr));
   else
#endif
    data=(a_ptr *)realloc(data,size*sizeof(a_ptr));
    if(data)
    {

        memcpy(data,other->data,other->size*sizeof(a_ptr));
        //now place the new data
        memcpy(&data[other->size],copy.data,oldsize*sizeof(a_ptr));
    }
    else
    {
        data=oldpointer;
        size=oldsize;
    }
}

void gArrayPrivate::insertAtFirst(const void *pData, gu32 blocksize, gu32 arrSize){
    lo;
    gu32 oldsize;
    a_ptr *oldpointer;
    unsigned char *c;
    gu32 i;
    gArrayPrivate copy;//make a copy

    if(!pData)return;
    oldsize=size;
    oldpointer=data;

    copy.insertAtEnd(this);
    size+=arrSize;
#ifdef GUSE_VIRTUALMEMORY
   if(vm)
       data=(a_ptr *)vm->vmReAlloc(data,size *sizeof(a_ptr));
   else
#endif
    data=(a_ptr *)realloc(data,size*sizeof(a_ptr));
    if(data)
    {
        c=(unsigned char *)pData;
        for(i=0;i<arrSize;i++)
        {
            data[i]=ac(&c[i*blocksize]);
        }
        //Now copy the old data
        memcpy(&data[arrSize],copy.data,copy.size*sizeof(a_ptr));
    }
    else
    {
        data=oldpointer;
        size=oldsize;
    }

}

//Tricky now
void gArrayPrivate::insert(const void *pData, gu32 index){
    lo;
    gArrayPrivate low;
    gArrayPrivate high;
    gu32 oldsize;
    a_ptr *oldpointer;
    if(!data){
        insertAtEnd(pData);//Empty array
        return;
    }
    //verify index to figure it out where the data is going to be placed
    if(index==0)//As position 0 means at start so just call insertAtFirst
    {
        insertAtFirst(pData);
        return;
    }
    else if(index>=size)//Index is bigger or equal than size then we insert at end
    {
        insertAtEnd(pData);
        return;
    }
    else if(index>0 && index < size)//index is on the middle
    {
        //in the middle
        //Split the array using index as reference
        low.alloc(index);//low contains from 0 to index - 1
        memcpy(low.data,data,low.size*sizeof(a_ptr));//copy to low from 0 to index - 1
        //now get data from size-index
        high.alloc(size-index);//high contains from index to size - 1
        memcpy(high.data,&data[index],high.size*sizeof(a_ptr));
        //we have both ends now we realloc the array
        oldsize=size;
        oldpointer=data;
        size++;
#ifdef GUSE_VIRTUALMEMORY
   if(vm)
       data=(a_ptr *)vm->vmReAlloc(data,size *sizeof(a_ptr));
   else
#endif
        data=(a_ptr *)realloc(data,size*sizeof(a_ptr));
        if(data)
        {
            //this step may be unnecessary
            memcpy(data,low.data,sizeof(a_ptr)*low.size);
            //set our data
            data[index]=ac(pData);
            //copy the tail of array starting from index + 1
            memcpy(&data[index+1],high.data,sizeof(a_ptr)*high.size);
        }
        else
        {
            data=oldpointer;
            size=oldsize;
        }

    }
}


void gArrayPrivate::insert(const gArrayPrivate *other,gu32 index)
{
    lo;

    gArrayPrivate low;
    gArrayPrivate high;

    gu32 oldsize;
    a_ptr *oldpointer;
    if(!other->data)return;
    if(!data)
    {
        insertAtEnd(other);//Empty array
        return;
    }
    if(index==0)
    {
        insertAtFirst(other);
        return;
    }
    else if(index>=size)
    {
        insertAtEnd(other);
        return;
    }
    else if(index>0 && index < size)
    {
        //in the middle
        //ok data from 0 to pos
        low.alloc(index);
        memcpy(low.data,data,low.size*sizeof(a_ptr));
        //now get data from size-index+1
        high.alloc(size-index);
        memcpy(high.data,&data[index],high.size*sizeof(a_ptr));
        //we have both ends now we realloc the damn array
        oldsize=size;
        oldpointer=data;
        size+=other->size;
#ifdef GUSE_VIRTUALMEMORY
   if(vm)
       data=(a_ptr *)vm->vmReAlloc(data,size *sizeof(a_ptr));
   else
#endif
        data=(a_ptr *)realloc(data,size*sizeof(a_ptr));
        if(data)
        {
            memcpy(data,low.data,sizeof(a_ptr)*low.size);
            memcpy(&data[index],other->data,sizeof(a_ptr)*other->size);
            memcpy(&data[index+other->size],high.data,sizeof(a_ptr)*high.size);
        }
        else
        {
            data=oldpointer;
            size=oldsize;
        }

    }
}

void gArrayPrivate::insert(const void *pData, gu32 blockSize, gu32 arrSize, gu32 index)
{
    lo;

    gArrayPrivate low;
    gArrayPrivate high;

    gu32 oldsize,i;
    a_ptr *oldpointer;
    unsigned char *c;
    if(!pData || !blockSize || !arrSize)
        return;
    if(!data)
    {
        insertAtEnd(pData,blockSize,arrSize);//Empty array
        return;
    }
    if(index==0)
    {
        insertAtFirst(pData,blockSize,arrSize);
        return;
    }
    else if(index>=size)
    {
        insertAtEnd(pData,blockSize,arrSize);
        return;
    }
    else if(index>0 && index < size)
    {
        //in the middle
        //ok data from 0 to pos
        low.alloc(index);
        memcpy(low.data,data,low.size*sizeof(a_ptr));
        //now get data from size-index+1
        high.alloc(size-index);
        memcpy(high.data,&data[index],high.size*sizeof(a_ptr));
        //we have both ends now we realloc the damn array
        oldsize=size;
        oldpointer=data;
        size+=arrSize;
#ifdef GUSE_VIRTUALMEMORY
   if(vm)
       data=(a_ptr *)vm->vmReAlloc(data,size *sizeof(a_ptr));
   else
#endif
        data=(a_ptr *)realloc(data,size*sizeof(a_ptr));
        if(data)
        {
            memcpy(data,low.data,sizeof(a_ptr)*low.size);
            c=(unsigned char *)pData;
            for(i=0;i<arrSize;i++)
            {
                data[index+i]=ac(&c[i*blockSize]);
            }
            memcpy(&data[index+arrSize],high.data,sizeof(a_ptr)*high.size);
        }
        else
        {
            data=oldpointer;
            size=oldsize;
        }

    }
}

//
void *gArrayPrivate::getData(gu32 index)
{
    //too much checking?
    lo NULL;//locking check
    ie NULL;//index boundary check
    e NULL;//data pointer check
    return (void *)data[index];
}
const void *gArrayPrivate::constData(gu32 index) const
{
    ie NULL;
    e NULL;
    return (const void *)data[index];
}
//another headache
void *gArrayPrivate::removeAtFirst(gu32 blockSize,bool bRemove)
{
    //easy way just rebuild the array
    lo 0;
    e 0;
    gArrayPrivate copy;
    void *rval;
    gu32 oldsize;
    a_ptr *oldpointer;
    gArrayPrivate btr;
    if(blockSize==0)return 0;
    if(blockSize>=size)
    {
        rval=(void *)data[0];
        clear(bRemove);
        if(!bRemove)
            return rval;
         else
            return 0;
    }
    copy.alloc(size-blockSize);
    memcpy(copy.data,&data[blockSize],copy.size * sizeof(a_ptr));

    oldsize=size;
    oldpointer=data;
    size-=blockSize;
    rval=(void *)data[0];
    if(bRemove)
    {
        btr.alloc(blockSize);
        memcpy(btr.data,data,blockSize*sizeof(a_ptr));
    }
#ifdef GUSE_VIRTUALMEMORY
   if(vm)
       data=(a_ptr *)vm->vmReAlloc(data,size *sizeof(a_ptr));
   else
#endif
    data=(a_ptr *)realloc(data,size * sizeof(a_ptr));
    if(data)
    {
        memcpy(data,copy.data,size * sizeof(a_ptr));
        if(bRemove && hascleaning())
        {
            if(factory)
                btr.factory=factory;
            else if(func)
                btr.func = func;
            return 0;
        }
        else
        {
            return rval;
        }


    }
    else
    {
        size=oldsize;
        data=oldpointer;
        return 0;
    }


}

void *gArrayPrivate::removeAtEnd(gu32 blockSize,bool bRemove)
{
    lo 0;
    e 0;

    void *rval;
    gu32 oldsize;
    a_ptr *oldpointer;
    gArrayPrivate btr;
    if(blockSize==0)return 0;
    if(size==1 || blockSize>=size)
    {
        rval=(void *)data[0];
        clear(bRemove);
        if(!bRemove)
            return rval;
        else
            return 0;
    }
    oldsize=size;
    oldpointer=data;
    rval=(void *)data[size-blockSize];
    if(bRemove)
    {
        btr.alloc(blockSize);
        memcpy(btr.data,&data[size-blockSize],blockSize*sizeof(a_ptr));
    }
    size-=blockSize;
#ifdef GUSE_VIRTUALMEMORY
   if(vm)
       data=(a_ptr *)vm->vmReAlloc(data,size *sizeof(a_ptr));
   else
#endif
    data=(a_ptr *)realloc(data,size * sizeof(a_ptr));
    if(data)
    {
        if(bRemove && hascleaning())
        {
            if(factory)
                btr.factory=factory;
            else if(func)
                btr.func = func;
            return 0;
        }
        else
            return rval;
    }
    else
    {
        data=oldpointer;
        size=oldsize;
        return 0;
    }
}

void *gArrayPrivate::removeAt(gu32 index,gu32 blockSize, bool bRemove)
{
    lo 0;
    e 0;
    ie 0;
    gArrayPrivate low;
    gArrayPrivate high;
    gArrayPrivate btr;
    gu32 oldsize;
    a_ptr *oldpointer;
    void *rval;
    if(blockSize==0)return 0;
    if(index>=size || index+blockSize>size)return 0;
    if(size==1 || blockSize>=size)
    {
        rval=(void *)data[0];
        clear(bRemove);
        if(!bRemove)
            return rval;
        else
            return 0;
    }
    if(index==0)
    {
        return removeAtFirst(blockSize,bRemove);
    }
    else if(index==size - 1 || index+blockSize==size)
    {
        return removeAtEnd(blockSize,bRemove);
    }
    else if(index > 0 && index < size - 1)
    {
        low.alloc(index);//The low since we are 0 based indexes  size will be -1
        memcpy(low.data,data,sizeof(a_ptr)* low.size);

        high.alloc(size-index-blockSize);
        memcpy(high.data,&data[index+blockSize],sizeof(a_ptr) * high.size);
        rval=(void *)data[index];

        //save our block to be removed
        if(bRemove)
        {
            btr.alloc(blockSize);
            memcpy(btr.data,&data[index],sizeof(a_ptr) * blockSize);
        }

        oldsize=size;
        oldpointer=data;
        size-=blockSize;
#ifdef GUSE_VIRTUALMEMORY
   if(vm)
       data=(a_ptr *)vm->vmReAlloc(data,size *sizeof(a_ptr));
   else
#endif
        data=(a_ptr *)realloc(data,size * sizeof(a_ptr));
        if(data)
        {
            memcpy(data,low.data,sizeof(a_ptr)*low.size);
            memcpy(&data[low.size],high.data,sizeof(a_ptr) * high.size);
            if(bRemove && func){
                if(factory)
                    btr.factory=factory;
                else if(func)
                    btr.func = func;
                return 0;
            }
            else
                return rval;
        }
        else
        {
            data=oldpointer;
            size=oldsize;
            return 0;
        }


    }
    else
        return 0;
}

void gArrayPrivate::resize(gu32 newsize, bool bRemove)
{
    lo;
    e;

    gu32 oldsize;
    a_ptr *oldpointer;
    gArrayPrivate btr;//remanents if any

    oldsize=size;
    oldpointer=data;

    if(newsize<size && bRemove)
    {
        btr.alloc(size-newsize);
        memcpy(btr.data,&data[newsize],sizeof(a_ptr)*btr.size);
    }

    size=newsize;
#ifdef GUSE_VIRTUALMEMORY
   if(vm)
       data=(a_ptr *)vm->vmReAlloc(data,size *sizeof(a_ptr));
   else
#endif
    data=(a_ptr *)realloc(data,sizeof(a_ptr)*size);
    if(data)
    {
        if(bRemove && newsize<oldsize)
        {
            if(factory)
                btr.factory=factory;
            else if(func)
                btr.func = func;
        }else if(oldsize<newsize){
            //we are increasing the array be smart and set to 0 newly created values
            memset(&data[oldsize],0,(newsize - oldsize)*sizeof(a_ptr));
        }
    }
    else
    {
        data=oldpointer;
        size=oldsize;
    }

}
void gArrayPrivate::copy(const gSharedObject *other){
    const gArrayPrivate *obj=static_cast<const gArrayPrivate *>(other);
    //gassert(obj->data,"gArrayPrivate data is null");
    if(!obj)return;
    if(!obj->data)return;
    bAvoidDuplicates=obj->bAvoidDuplicates;
    bLocked=obj->bLocked;

    afunc = obj->afunc;

    if(alloc(obj->size,obj->blocksize)){
        if(obj->afunc || obj->factory){
            a_ptr val;
            //make a deep copy of each array element
            for(gu32 i=0;i<size;i++){
                if(obj->factory)
                    val = (a_ptr)obj->factory->create((const void *)obj->data[i]);
                else if(obj->afunc){
                    val = (a_ptr)afunc((const void *)obj->data[i]);
                }
                data[i] = val;
            }
        }else{
            memcpy(data,obj->data,size * sizeof(a_ptr));
        }
    }
    factory = obj->factory;
    func=obj->func;

}



