#include "gVirtualMemoryPrivate.h"
using namespace gfw;
#define ca gVirtualMemoryPrivate *o=(gVirtualMemoryPrivate *)d
#define co gVirtualMemoryPrivate *io=(gVirtualMemoryPrivate *)other.d
gVirtualMemory::gVirtualMemory():gBaseShared(new gVirtualMemoryPrivateFactory){

}

gVirtualMemory::gVirtualMemory(const gVirtualMemory &other):gBaseShared(other,new gVirtualMemoryPrivateFactory){

}

gVirtualMemory::~gVirtualMemory()
{

}

//Create
bool gVirtualMemory::create(gu32 _size, void *memSpace)
{
    //this is simple
    clear();
    gVirtualMemoryPrivate *o=new gVirtualMemoryPrivate(_size,memSpace==0);
    if(memSpace){
        o->dataArray=(gu8 *)memSpace;
    }

    d=o;
    return o->dataArray!=0;
}

//Here's comes the good one
void *gVirtualMemory::vmAlloc(gu32 _size, gu32 *virtSpace)
{
    if(!d)return 0;
    ca;
    gvmem_block_header *ret;
    gvmem_alloc_element *el,*previous;
    gu32 ubSize,uNewAddress,dwSize;
    gu32 i=0;
    if(_size > o->freeMemory())
        return 0;

    if(o->allocationTable.isEmpty()){//allocation table is empty
        //first element of list is the header

        el=new gvmem_alloc_element(0,0);
        o->allocationTable.append(el);
        ubSize=1;

    }
    else{

        ubSize=o->allocationTable.size();
    }
    //here we try to find whether it is a space between blocks where we may allocate memory
    //This is not a thread safe routine, multiples objects accessing this routine from the same memory object
    //could cause trouble and locking this resource would for sure slow the process
    for(i=0;i<ubSize;i++){
        previous=0;
        el=o->allocationTable.pValue(i);
        if(i>0)
            previous=o->allocationTable.pValue(i-1);//get the previous block
        //try to find out if there is a space either between the previous and the next
        if(previous){
           uNewAddress=previous->blockAddress+previous->blockSize;//get a potential new address which just step ahead of previous block
           dwSize=el->blockAddress - sizeof(gvmem_block_header) - uNewAddress;//get gap in bytes between the current block and previous
           if(sizeof(gvmem_block_header)+ _size <=dwSize){ // if the gap between blocks suits the size of for a new spece then we add it
               //we have a block here then
              el=new gvmem_alloc_element(_size,uNewAddress+sizeof(gvmem_block_header));
               o->allocationTable.insert(el,i);//we insert any new element at a position so we can have synch
               o->updateAddresses(i,true);//update addresses
               ret=(gvmem_block_header *)&o->dataArray[uNewAddress];//get the address of the block header
               ret->bSize=sizeof(gvmem_block_header);//size of the block header
               ret->uAddress=i;//address or position in allocation table
               o->usedMemory+=_size + sizeof(gvmem_block_header);//increase memory used

               if(virtSpace)
                   *virtSpace=el->blockAddress;//in case we want the virtual space address
               return (void *)&o->dataArray[el->blockAddress];
           }
        }



     }

    previous=o->allocationTable.pValue(ubSize-1);
    uNewAddress=previous->blockAddress+previous->blockSize;

    ret=(gvmem_block_header *)&o->dataArray[uNewAddress];
    ret->bSize=sizeof(gvmem_block_header);
    ret->uAddress=ubSize;
    o->usedMemory+=_size + sizeof(gvmem_block_header);
    gu32 newBlockAddress= uNewAddress + sizeof(gvmem_block_header);
    //check this address + size is not going further than the virtual memory size
    if((newBlockAddress + _size) >= o->size){
        //could happen if there are gaps between blocks
        //TODO: A function closing gaps
        return 0;
    }
    el=new gvmem_alloc_element(_size,newBlockAddress);
    o->allocationTable.append(el);
    if(virtSpace)
        *virtSpace=el->blockAddress;

    return (void *)&o->dataArray[el->blockAddress];

}
//realloc implementation
void *gVirtualMemory::vmReAlloc(void *vPtr, gu32 _size, gu32 *virtAddress){
    if(!d)return 0;
    ca;
    //for this func we need only to check i there is enough space ahead from the current memory block
    //if so we only expand the memory block or else we allocate a new space and deallocate the old one
    if(!vPtr){ //no memory pointer set just call alloc
        return vmAlloc(_size,virtAddress);
    }
    if(_size > o->freeMemory())return 0;//not enought memory

    gvmem_block_header *bh=(gvmem_block_header *)vPtr;
    bh--;
    if(!o->verify_block(bh))return 0;//block is damaged or corrupt
    gvmem_alloc_element *be=o->allocationTable.pValue(bh->uAddress);//get th descriptor header from allocation table
    //check if we set the new size
    if(bh->uAddress == o->allocationTable.size() - 1){//block is at the end
        //just set the size
        if(be->blockAddress + _size > o->size)return 0;//we cannot go further
        o->usedMemory-= be->blockSize;
        be->blockSize = _size;
        o->usedMemory+=_size;
        if(virtAddress)*virtAddress=be->blockAddress;
        return vPtr;
    }else{
        //check the next block ahead this block
        void *ret;
        gvmem_alloc_element *nel=o->allocationTable.pValue(bh->uAddress + 1);
        gu32 nuaddress=nel->blockAddress - sizeof(gvmem_block_header); // the address of the block header
        gu32 uaddress=be->blockAddress + _size;
        if(uaddress > nuaddress){//not space alloc new place
            //need a copy of this block chunk
            gMemory mem(vPtr,sizeof(char),be->blockSize,true);
            gvmem_alloc_element temp=*be;//save it as vmFree kills it
            vmFree(vPtr);//free space
            ret=vmAlloc(_size,virtAddress);//alloc first
            //now copy all old data there
            if(_size > be->blockSize)
                memcpy(ret,mem.data(),temp.blockSize);
            else
                memcpy(ret,mem.data(),_size);
            return ret;
        }else{
            //there is space
            o->usedMemory-=be->blockSize;
            be->blockSize=_size;
            o->usedMemory+=_size;
            if(virtAddress)*virtAddress=be->blockAddress;
            return vPtr;
        }
    }
    return 0;//we should never get here but to avoid VC compiler warning
}

void gVirtualMemory::vmFree(void *mem)
{
    if(!d)return;
    if(!mem) return;
    ca;
    gvmem_block_header *bh;
    gvmem_alloc_element *el;
    bh=(gvmem_block_header *)mem;
    bh--;

    if(o->verify_block(bh))
    {

        el=o->allocationTable.pValue(bh->uAddress);
        o->usedMemory-=(el->blockSize + sizeof(gvmem_block_header));
        //make sure that next addresses beyond next address get updated
        o->updateAddresses(bh->uAddress,false);
        o->allocationTable.remove(bh->uAddress,1);
        bh->bSize=0;
        bh->uAddress=0;   
    }
}



gu32 gVirtualMemory::size() const
{
    if(!d)return 0;
    ca;
    return o->size;
}

gu32 gVirtualMemory::usedMemory() const
{
    if(!d)return 0;
    ca;
    return o->usedMemory;
}


gu32 gVirtualMemory::freeMemory() const
{
    if(!d)return 0;
    ca;
    return o->freeMemory();
}

void *gVirtualMemory::data() const
{
    if(!d)return 0;

    ca;
    return o->dataArray;
}



gu32 gVirtualMemory::blockHeaderSize()
{
    return sizeof(gvmem_block_header);
}

gu32 gVirtualMemory::getAllocationCount()const{
    if(!d)return 0;
    ca;
    if(o->allocationTable.isEmpty())return 0;
    return o->allocationTable.size() - 1;
}
gVirtualMemory &gVirtualMemory::operator=(const gVirtualMemory &other)
{
    assign(other);
    return *this;
}

void gVirtualMemory::printMemory() const{
    ca;
    if(!d)return;
    gvmem_alloc_element *el;
    if(o->allocationTable.isEmpty()){
        std::cout << "No memory Allocated " << std::endl;return;
    }
    std::cout << "Memory Report: Total Allocations: " << o->allocationTable.size() - 1 << std::endl;
    for(gu32 i=1;i<o->allocationTable.size();i++){
        el=o->allocationTable.pValue(i);
        std::cout << "Memory Block " << i << " size " << el->blockSize << " virtual address " << el->blockAddress << std::endl;
    }
    std::cout << "Used Memory : " << o->usedMemory << std::endl;
    std::cout << "Free Memory : " << o->freeMemory() << std::endl;
    std::cout << "Total Memory :" << o->size << std::endl;
}

gu32 gVirtualMemory::getVirtualMemLocation(const void *mem) const{
    if(!d)return 0;
    ca;
    gvmem_block_header *header=(gvmem_block_header *)mem;
    gvmem_alloc_element *el;
    header--;
    if(!o->verify_block(header))return 0;
    el=o->allocationTable.pValue(header->uAddress);
    return el->blockAddress;
}
gu32 gVirtualMemory::getVirtualMemBlockSize(const void *mem) const{
    if(!d)return 0;
    ca;
    gvmem_block_header *header=(gvmem_block_header *)mem;
    header--;
    if(!o->verify_block(header))return 0;
    gvmem_alloc_element *el=o->allocationTable.pValue(header->uAddress);
    return el->blockSize;
}

bool gVirtualMemory::isVirtualMemoryBlock(const void *mem) const{
    if(!d)return false;
    ca;
    gvmem_block_header *block=(gvmem_block_header *)mem;
    block--;
    return o->verify_block(block);
}
