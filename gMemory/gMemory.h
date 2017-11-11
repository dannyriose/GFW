#ifndef GMEMORY_H
#define GMEMORY_H
#include <gfw_global.h>
#include <gVirtualMemory/gVirtualMemory.h>
namespace gfw{

    //! Typedef function to work as call back for constructing and destructing memory blocks or array members.
    typedef void(*ICDCallback)(void *ptr);
    enum GMEMORY_ALLOCATORS{
        //! Standard C heap memory allocation function malloc,free,realloc
        GMEM_STDALLOC,
        //! gVirtualMemory allocation functions.
        GMEM_GVMALLOC
    };
    //! class gMemory is an utilty object that works as base class for memory handling.
    /** Represents a block or chunk of contigous memory, similar to a dynamic array.
      Unlike the class gArray, gMemory does not use an array of pointers but an array
      bytes making it ideal to make primitive memory blocks of any size.
      gMemory class works as base of primitive array templates as well as for
      user type templates. See gBaseArray template file.*/
    class SHARED_GFW gMemory:public gBaseShared
    {
    public:
        gMemory();
        gMemory(bool bShared);
        gMemory(const gMemory &other);
        gMemory(const void *data, gu32 blockSize,gu32 size,bool bCopy);
        ~gMemory();
        //Allocator
        bool alloc(gu32 blockSize,gu32 Size,int type=GMEM_STDALLOC);

        //reading
        //entire array
        void *data() const;
        //Section of the array by blockSize
        void *data(gu32 index) const;
        //An specific offset
        void *offset(gu32 lOffset) const;
        gu32 size() const;
        gu32 blocksize() const;
        gu32 sizeInBytes() const;
        //Writing
        //! Set a datablock or array of memory blocks as the gMemory data.
        /** \param data: Pointer to data block.
            \param blockSize: sizeof each array element contained in data.
            \param Size: total count of elements on data.
            \param bCopy: Indicates if data is block is going to be copied or just passed as a pointer.
            \param dataRefCount: This sets a flag telling the object it will take ownership of data.
                             therefore freeing any memory used from it. Not really recommended.
                             This only have effect when bCopy is false.
            \param type: Allocation type. */
        void setData(const void *data,
                     gu32 blockSize,
                     gu32 Size,
                     bool bCopy=true,
                     bool bTakeDataOwnership=true,
                     gs32 type=GMEM_STDALLOC);
        void setData(const gMemory &other,bool bCopy=true, int type=GMEM_STDALLOC);
        //Convenience operator
        void *operator [] (int index) const;
        gMemory &operator=(const gMemory &other);
        operator const void*() const;
        //
        virtual bool isEmpty() const;
        void memsetTo(gu8 value);
        //did not want to but had no choice
        void insertAtEnd(const gMemory &other);
        void insertAtEnd(const void *data,gu32 Size, gu32 blockSize);
        gMemory section(gu32 index,gu32 _size, bool newBlock=0) const;


    };
}
#endif // GMEMORY_H
