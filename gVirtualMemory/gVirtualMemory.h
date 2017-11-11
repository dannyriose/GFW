#ifndef GVIRTUALMEMORY_H
#define GVIRTUALMEMORY_H


#include <gfw_global.h>
namespace gfw
{
    class SHARED_GFW gVirtualMemory:public gBaseShared{
    public:
        gVirtualMemory();
        gVirtualMemory(const gVirtualMemory &other);
        ~gVirtualMemory();
        //typicals
        bool create(gu32 _size, void *memSpace=0);//create the virtual memory
        //virt space gets the virtual address on the memory, and returns the physical address
        void *vmAlloc(gu32 _size,gu32 *virtAddress=0);//Allocates a block inside the virtual memory space
        void *vmReAlloc(void *vPtr,gu32 _size,gu32 *virtAddress=0);
        void vmFree(void *mem);//de allocates the memory block


        gVirtualMemory &operator=(const gVirtualMemory &other);

        //states
        gu32 size() const;
        gu32 freeMemory() const;
        gu32 usedMemory() const;
        static gu32 blockHeaderSize();
        gu32 getAllocationCount() const;
        gu32 getVirtualMemLocation(const void *mem) const;
        gu32 getVirtualMemBlockSize(const void *mem) const;
        bool isVirtualMemoryBlock(const void *mem) const;
        void *data() const;
        //data memory printing
        void printMemory() const;

    };
}
#endif // GVIRTUALMEMORY_H
