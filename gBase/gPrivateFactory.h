#ifndef GPRIVATEFACTORY_H
#define GPRIVATEFACTORY_H
#include "gSharedObject.h"
#include "gSharedObjectFactory.h"
namespace gfw{

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


}


#endif // GPRIVATEFACTORY_H
