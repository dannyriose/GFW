/*********************************GVM.H**************************************************/
/*           FILE: GVM.H                                                                */
/*           DESCRIPTION: gVM or gVirtualMachine is a bytecode configurable virtual machine */

#ifndef GVM_H
#define GVM_H
#include <gString/gString.h>
#include <gMemory/gBaseArray.h>
#include <gVirtualMemory/gVirtualMemory.h>
#include <gArray/gList.h>

#define GVM_PIDSTART 1000
#define GVM_PIDINCREMENT 10
namespace gfw
{

//A lists of stored procedures
//!Represents a procedure or proc from a program
    struct SHARED_GFW gVmProcItem
    {
        gString procName;//Proc name
        gu32 procAddress;//Proc Address
        gVmProcItem &operator=(const gVmProcItem &other)
        {
            procName=other.procName;
            procAddress=other.procAddress;
            return *this;
        }
    };
    //! Contains a list stored procs coming from a program
    class SHARED_GFW gVmProcList
    {
    private:
        gList<gVmProcItem> procs;//List of procs
    public:
        gVmProcList();
        //! Adds a proc to the list
        gVmProcItem *addProc(const gString &name, gu32 address);
        //! Returns a proc given an index returns 0 if not found
        gVmProcItem *getProc(gu32 index);
        gu32 procCount() const;//Procedure count
        bool isEmpty() const;

        void alloc(gu32 size);
        void setProc(gu32 index,gVmProcItem *item);
        gVmProcList &operator=(const gVmProcList &other);
        const gList<gVmProcItem> &list() const
        {
            return (const gList<gVmProcItem>&)procs;
        }

    };
    //! Represents a program instance of the virtual machine
    struct SHARED_GFW gVmInstance
    {
        gByteArray programMemory;//Bytecode VM
        gVirtualMemory dataMemory;//Data memory
        gVirtualMemory stack;//Stack Memory
        //Added this just to backup CPU registers
        //leaving along the stack
        gUIntArray regs;//Registers
        gu32 ssp;//start stack pointer when entering a funciton
        //counters
        gu32 pc;//program counter
        gu32 sp;//stack pointer
        gu32 fsp;//function stackpointer
        //Old a procedure list
        gVmProcList procs;
        bool isActive;
        gu32 pid;//process id
        gVmInstance():ssp(0),pc(0),sp(0),fsp(0),isActive(0),pid(0)
        {

        }

        gVmInstance &operator=(const gVmInstance &other);

        gu32 memAlloc(gu32 size);
        void memFree(void *mem);
        //stack helpers



    };
    //! gVm is a versatile and configurable byte virtual machine.
    class SHARED_GFW gVm:public gBaseShared{
    public:
        gVm();
        gVm(const gVm &other);
        ~gVm();
        //Creates the machine asgining the virtual memory size
        bool create(gu32 ramSize);
        //Loads a program instance from a file
        gVmInstance *load(const gString &gVMProgramFile, bool bCreateMemory=true, bool isActive=true);
        void addInstance(gVmInstance *instance);//Adds a gVmInstance to the program process list
        gVmInstance *getInstanceByIndex(gu32 index);
        gVmInstance *getInstanceById(const gString &id);
        gVmInstance *getInstanceById(gu32 id);

        void removeInstanceByIndex(gu32 index);
        void removeInstanceById(const gString &id);
        void removeInstanceById(gu32 id);
        //restart the PC to its start position, reallocates memory and make it active
        void resetInstanceByIndex(gu32 index);
        void resetInstanceById(gu32 id);
        void resetInstance(gVmInstance *instance);

        //Execution
        int exec();
        void execSingleCycle();//runs a single cycle
        void execFunction(const gString &funcName);
        void stop();//stops the machine execution
        void start();//stat the machine if paused


        virtual void loopCallback(){}
        //Virtual function to define custom instructions or special api function calls
        virtual void callc(gu32 apiId,gu8 *stack,gu32 &sp,gVmInstance *instance){}
        //A process ended
        virtual void processEnded(gVmInstance *instance,gu32 instanceIndex){}

        //some helpers
        static inline gu32 pop32uint(gu8 *stack,gu32 &sp)
        {
            sp-=4;
            return *(gu32 *)&stack[sp];
        }

        static inline float pop32float(gu8 *stack,gu32 &sp)
        {
            sp-=4;
            return *(float *)&stack[sp];
        }
        static inline void push32uint(gu32 val,gu8 *stack,gu32 &sp)
        {
            gu32 *v=(gu32 *)&stack[sp];
            sp+=4;
            *v=val;
        }
        static inline void push32float(float val,gu8 *stack,gu32 &sp)
        {
            float *v=(float *)&stack[sp];
            sp+=4;
            *v=val;
        }

    };
}

#endif // GVM_H
