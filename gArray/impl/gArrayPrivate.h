#ifndef GARRAYPRIVATE_H
#define GARRAYPRIVATE_H
#include <gArray/gArray.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
namespace gfw{
//a_ptr typedef is the base type used to store array elements on gArray so its size depends on the compiler
//for 32 bit platforms this a_ptr is 32 bits for 64 bits a_ptr is 64.
typedef guptr a_ptr;
//ba is the private member m_d on class gArray
struct gArrayPrivate:public gSharedObject
{
   //! The size of this array or total count
   gu32 size;
   //! The size that each array element has
   gu32 blocksize;
   //! The array data
   a_ptr *data;
   //! Array is locked or not
   bool bLocked;
   //! Clean up function per array element
   IClean func;//Pointer to the cleaning function
   //! Allocation function per array element
   IAlloc afunc;
   //! Factory if any
   gArrayElementFactory *factory;
   gArrayPrivate();//simpre constructor
   ~gArrayPrivate();//simple destructor
   bool bAvoidDuplicates;
#ifdef GUSE_VIRTUALMEMORY
   gVirtualMemory *vm;//pointer to virtual memory object
#endif

//These are the same members as explained on gArray.h
   bool alloc(gu32 usize,gu32 blocksize=0);
   void clear(bool bRemove=true);
   void setData(void *pdata,gu32 index,bool removeOld);
   void insertAtEnd(const void *pData);
   void insertAtEnd(const gArrayPrivate *other);
   void insertAtEnd(const void *pData,gu32 blocksize, gu32 arrSize);
   //
   void insertAtFirst(const void *pData);
   void insertAtFirst(const gArrayPrivate *other);
   void insertAtFirst(const void *pData,gu32 blocksize,gu32 arrSize);

   void insert(const void *pData,gu32 index);
   void insert(const gArrayPrivate *other, gu32 index);
   void insert(const void *pData,gu32 blockSize,gu32 arrSize,gu32 dwIndex);

   void *getData(gu32 index);
   const void *constData(gu32 index) const;
   gu32 getSize(){return size;}
   void* removeAtFirst(gu32 blockSize, bool bRemove);
   void* removeAtEnd(gu32 blockSize, bool bRemove);
   void* removeAt(gu32 index,gu32 blockSize, bool bRemove);

   void resize(gu32 newsize,bool bRemove);

   virtual void copy(const gSharedObject *other);
   void elremove(a_ptr element);
   ginline bool hascleaning(){return func!=0 || factory!=0;}

};

    G_SHAREDOBJECT_FACTORY(gArrayPrivateFactory,gArrayPrivate);

}
#endif // GARRAYPRIVATE_H
