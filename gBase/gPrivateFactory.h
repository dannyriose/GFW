#ifndef GPRIVATEFACTORY_H
#define GPRIVATEFACTORY_H

namespace gfw{

struct gSharedObject;


//! Abstract class designed to work as the memory allocator for the shared object.
struct gSharedObjectFactory{
    //! Allocates the shared object. This must be reimplemented as new ObjectDerivedFromgSharedObject
    virtual gSharedObject *create() const = 0;
    //! Deallocates memory from the shared object. same as delete obj.
    /** The idea is to avoid issues with windows DLLs. DLLs have their own heap pool so if the DLL allocates
     the shared object then it should deallocate it*/
    virtual void remove(gSharedObject *obj) const = 0;
    //! Deallocates the memory used by the object factory.. same as delete this. Resolves the issue of DLLs and HEAP
    virtual void deleteme() = 0;
    virtual ~gSharedObjectFactory(){

    }
};


template <class K>
struct gPrivateFactory: public gSharedObjectFactory{
    virtual gSharedObject *create() const{
        return new K();
    }
    virtual K *_new() const{
        return new K();
    }
    virtual void remove(gSharedObject *ref) const{
        delete ref;
    }
    virtual void _delete(K *ref) const{
        delete ref;
    }

    virtual void deleteme(){
        delete this;
    }

    ~gPrivateFactory(){}
};



//Macro that implements the sharedobject factory withing a single line
//Keep the rule the one making it the one frees it
#define G_SHAREDOBJECT_FACTORY(name,type) struct name:public gSharedObjectFactory{ \
                                             virtual gSharedObject *create() const{ \
                                                     return new type; \
                                              } \
                                             virtual void remove(gSharedObject *obj) const{\
                                                     delete obj; \
                                             } \
                                             virtual void deleteme(){\
                                                 delete this; \
                                             } \
                                              ~name(){\
                                              } \
                                         }

}


#endif // GPRIVATEFACTORY_H
