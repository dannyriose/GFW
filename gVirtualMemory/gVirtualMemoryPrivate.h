#ifndef GVIRTUALMEMORYPRIVATE_H
#define GVIRTUALMEMORYPRIVATE_H
#include "gVirtualMemory.h"
#include <gMemory/gBaseArray.h>
#include <gArray/gList.h>
#include <memory.h>
#include <iostream>
namespace gfw{
struct gvmem_block_header //the block header
{
    //important to know..the size of the block in bytes.this to verify if the block is not corrupted
    gu16 bSize;//18 bytes
    gu32 uAddress;//The block address from the allocation table
};

struct gvmem_alloc_element
{
    gu32 blockSize;
    gu32 blockAddress;

    gvmem_alloc_element():blockSize(0),blockAddress(0)
    {

    }
    gvmem_alloc_element(gu32 _blockSize,gu32 _blockAddress):blockSize(_blockSize),blockAddress(_blockAddress)
    {

    }
};

struct gVirtualMemoryPrivate:public gSharedObject
{
    //The most simple and basic data type to hold the memory object
    gByteArray data;
    //And the most
    gu8 *dataArray;//simplify and speed up everything containing data here
    gu32 size;
    gu32 usedMemory;
    bool bAutoCloseGaps;
    //verify if the block is not corrupted and if the block belongs to the virtual memory space
    //num blocks
    gList<gvmem_alloc_element> allocationTable;
    bool verify_block(gvmem_block_header *vh)
    {
        if(gsize(vh)>=gsize(dataArray) && gsize(vh) < gsize(&dataArray[size - 1]))
        {
            //ok lets check
            return vh->bSize==sizeof(gvmem_block_header);
        }
        else
        {
            return false;
        }

    }
    inline gu32 freeMemory()
    {
        return size - usedMemory;
    }
    gVirtualMemoryPrivate():
        dataArray(0),
        size(0),
        usedMemory(0),
        bAutoCloseGaps(0){

    }
    gVirtualMemoryPrivate(gu32 _size,bool bCreate):
        dataArray(0),
        size(_size),
        usedMemory(0),
        bAutoCloseGaps(0){
        if(bCreate){
            data.alloc(_size);
            dataArray=data.data();

        }
    }
    ~gVirtualMemoryPrivate(){

    }

    //closes possible gaps between memory blocks..expensive routine as memory blocks need to be moved
    void closeGaps(){
        gvmem_block_header *nextBlockHeader;
        gvmem_alloc_element *blockDescriptor,*nextBlockDescriptor;
        //by default the first block starts on 1
        gu32 allocationTableSize=allocationTable.size();
        unsigned char *nextDataArray;
        int addr;
        for(gu32 i=1;i<allocationTableSize;i++){
            blockDescriptor=allocationTable.pValue(i);
            //check if we have gaps
            if(i == allocationTableSize - 1) continue;//last block move on

            nextBlockDescriptor=allocationTable.pValue(i+1);
            addr=(int)(blockDescriptor->blockAddress + blockDescriptor->blockSize);
            addr-=(int)(nextBlockDescriptor->blockAddress - sizeof(gvmem_block_header));
            if(addr>0){
                //close the gap here
                //addr would be the new address of nextblock
                nextBlockHeader=(gvmem_block_header *)&dataArray[(nextBlockDescriptor->blockAddress - sizeof(gvmem_block_header))];
                nextDataArray=&dataArray[nextBlockDescriptor->blockAddress];
                //now move
                memcpy(&dataArray[addr],nextBlockHeader,sizeof(gvmem_block_header));
                nextBlockDescriptor->blockAddress=addr + sizeof(gvmem_block_header);
                memcpy(&dataArray[nextBlockDescriptor->blockAddress],nextDataArray,nextBlockDescriptor->blockSize);
                //now move the block
            }
        }
    }
    void updateAddresses(gu32 refIndex,bool bIncrease){
        gvmem_block_header *bh;
        gvmem_alloc_element *el;
        for(gu32 i=refIndex+1;i<allocationTable.size();i++){
            el=allocationTable.pValue(i);
            bh=(gvmem_block_header *)&dataArray[el->blockAddress - sizeof(gvmem_block_header)];
            if(!bIncrease)
                bh->uAddress=i-1;
            else
                bh->uAddress=i;
        }
    }
    virtual void copy(const gSharedObject *other){
        GFW_PRIVATE_CAST(o,other,gVirtualMemoryPrivate);
        if(o->data.isEmpty()){
            dataArray=o->dataArray;
        }else{
            data=o->data;
            dataArray=data.data();
        }
        usedMemory=o->usedMemory;
        bAutoCloseGaps=o->bAutoCloseGaps;
        allocationTable.copyFromOther(o->allocationTable);
    }


};
    G_SHAREDOBJECT_FACTORY(gVirtualMemoryPrivateFactory,gVirtualMemoryPrivate);
}
#endif // GVIRTUALMEMORYPRIVATE_H
