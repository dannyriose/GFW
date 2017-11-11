#ifndef GVMPRIVATE_H
#define GVMPRIVATE_H
#include <gVm/gVm.h>
#include <gVm/gVmIns.h>
#include <gVm/gVmAsm.h>
#include <stdlib.h>
#include <string.h>


//Macros
#define uintp(memobj,index) (gu32*)&memobj[index]
#define floatp(memobj,index) (float *)&memobj[index]
#define GET_L_BYTE(gu8) (gu8 & 0x0F)
#define GET_H_BYTE(gu8) ((gu8 & 0xF0)>>4)
#define MAX_REGISTERS 8
//registers
namespace gfw{
union gvm_register
{
    gu64 ru64;
    gs64 rs64;
    gf64 rf64;
    gu32 ru32[2];
    gs32 rs32[2];
    gf32 rf32[2];
    gu16 r16[4];
    gu8 r8[8];
};


struct gVmPrivate:public gSharedObject
{
    //lets set 8 registers
    enum paramtype
    {
        TYPE_REGISTER,
        TYPE_MEMADDRESS,
        TYPE_CONSTANT,
        TYPE_STACK, //Yes now access to the stack
        TYPE_STACKPOINTER,
        TYPE_STACKDIRECT
    };

    enum address_modes
    {
        DIRECT,INDIRECT
    };

    gvm_register r[MAX_REGISTERS];//the cpu registers
    gVirtualMemory ram;//ram memory
    gList<gVmInstance> programs;//The program list
    gu32 constantPointer[2];
    //Current program memory
    gu8 *progMemory;
    //current program data memory
    gu8 *dataMemory;
    //Current program stack
    gu8 *stack;
    //start stack pointer when enterin a function
    gu32 ssp;
    //Current stackpointer
    gu32 sp;
    //current program pointer
    gu32 pc;
    //current program index
    gu32 index;
    //current function stack pointer
    gu32 fsp;
    //get the memory pointer address
    bool bFirstParameterIsConstant;
    //not actually needed for copying
    bool brun;
    //pointer to current program
    gVmInstance *currentProg;
    //Kill ending processes
    bool bKillEndingProcesses;

    gu32 *address32(gu8 flag,gu32 addr){
        gu8 selection,addressMode;
        gu32 *ret;
        selection=GET_L_BYTE(flag);
        addressMode=GET_H_BYTE(flag);
        switch(selection){
        case TYPE_REGISTER:
            ret=&r[addr].ru32[0];
            break;
        case TYPE_MEMADDRESS:
            if(addressMode==DIRECT)
            {
                ret = (gu32 *)&dataMemory[addr];
            }
            else
            {
                constantPointer[0]=addr;
                ret=&constantPointer[0];
            }
            break;
        case TYPE_CONSTANT:
            if(!bFirstParameterIsConstant)
            {
                constantPointer[0]=addr;
                bFirstParameterIsConstant=true;
                ret=&constantPointer[0];

            }
            else
            {
                constantPointer[1]=addr;
                ret=&constantPointer[1];
                bFirstParameterIsConstant=false;
            }
            break;
        case TYPE_STACK:
            ret=(gu32 *)&stack[sp+addr];
            break;
        case TYPE_STACKPOINTER:
            ret=&sp;
            break;
        case TYPE_STACKDIRECT:
            ret=(gu32 *)&stack[addr];
        }
        if(addressMode!=DIRECT && selection!=TYPE_MEMADDRESS)
        {
            ret=(gu32 *)&dataMemory[*ret];
        }
        return ret;
    }
    inline void backUp32L()//backup data
    {
        int i;
        gu32 *data=currentProg->regs.data();
//old data
        /*for(i=MAX_REGISTERS -1 ;i>=0;i--)
        {
            data=uintp(stack,sp);
            *data=r[i].ru32[0];
            sp+=4;
        }*/
        //forget about annoying the stackpointer
        for(i=0;i<MAX_REGISTERS;i++){
            data[i]=r[i].ru32[0];
        }
        currentProg->pc=pc;
        currentProg->sp=sp;
        currentProg->fsp=fsp;
        currentProg->ssp=ssp;

    }

    inline void restore32L()
    {
        gu32 i;

        gVmInstance *ins=currentProg;
        pc=ins->pc;
        sp=ins->sp;
        ssp=ins->ssp;
        fsp=ins->fsp;
        dataMemory=(gu8 *)ins->dataMemory.data();
        stack=(gu8 *)ins->stack.data();
        progMemory=ins->programMemory.data();
        /*
        for(i=0;(i<MAX_REGISTERS) && sp!=0;i++)
        {
            sp-=4;
            r[i].ru32[0]=*uintp(stack,sp);
        }*/
        gu32 *data=ins->regs.data();
        for(i=0;i<MAX_REGISTERS;i++){
            r[i].ru32[0]=data[i];
        }
    }

    void copyFrom(const gVmPrivate &other)
    {
        bFirstParameterIsConstant=other.bFirstParameterIsConstant;
        constantPointer[0]=other.constantPointer[0];
        constantPointer[1]=other.constantPointer[1];
        dataMemory=other.dataMemory;
        stack=other.stack;
        progMemory=progMemory;
        sp=other.sp;
        pc=other.pc;
        fsp=other.fsp;
        index=other.index;
        for(gu32 i=0;i<MAX_REGISTERS;i++)
        {
            r[i]=other.r[i];
        }

        programs.copyFromOther(other.programs);
    }

    gVmPrivate():bFirstParameterIsConstant(0)
    {

    }

    gVmPrivate(const gVmPrivate &other)
    {
       copyFrom(other);
    }

    gVmPrivate &operator=(const gVmPrivate &other)
    {
        copyFrom(other);
        return *this;
    }
    ~gVmPrivate(){

    }
    virtual void copy(const gSharedObject *other){
        GFW_PRIVATE_CAST(o,other,gVmPrivate);
        copyFrom(*o);
    }

};
G_SHAREDOBJECT_FACTORY(gVmPrivateFactory,gVmPrivate);
}
#endif // GVMPRIVATE_H
