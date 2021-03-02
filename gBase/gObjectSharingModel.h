#ifndef GSHARED
#define GSHARED

#include "gSharedObject.h"

namespace gfw{

//! Base class for classes holding shared objects.
//!
template <class P,
          class F = gSharedObjectFactory>
class SHARED_GFW gObjectSharingModel{ //gObjectSharingModel
protected:
    P *d;
    F *d_factory;
public:
    gObjectSharingModel(F *_d_factory):d(0),d_factory(_d_factory){

    }
    gObjectSharingModel(const gObjectSharingModel<P,F> & other,F *_d_factory):
        d(0),d_factory(_d_factory){
         if(!other.d)return;
         switch(other.d->shareMode){
         case GSHARED_OBJECT_NOTSHARED:
             d=static_cast<P*>(d_factory->create());
             d->copy(other.d);
             break;
         case GSHARED_OBJECT_ALWAYS_SHARED:
         case GSHARED_OBJECT_SHARED:
             d=other.d;
             d->refCount++;
             break;

         }
    }
    void assign(const  gObjectSharingModel<P,F> &other){
        if(!other.d)return;
         clear();
         switch(other.d->shareMode){
         case GSHARED_OBJECT_NOTSHARED:
             d=static_cast<P*>(d_factory->create());
             d->copy(other.d);break;
         case GSHARED_OBJECT_ALWAYS_SHARED:
         case GSHARED_OBJECT_SHARED:
             d=other.d;
             d->refCount++;
             if(d->shareMode == GSHARED_OBJECT_SHARED)
                 d->shareMode=GSHARED_OBJECT_NOTSHARED;
             break;

         }
    }
    //! @todo Set clean up routine here and remove function clear as virtual functions on destructors are not reliable
    virtual ~gObjectSharingModel(){
        clearPrivate();
        if(d_factory)
             d_factory->deleteme();
    }


    virtual void clear(){
        clearPrivate();
    }
    inline void addRef() const {d->refCount++;}
    inline void decRef() const {d->refCount--;}
    inline gs32 referenceCount() const {return d->refCount;}
    inline void setShared(bool bSet) const {
        if(d->shareMode==GSHARED_OBJECT_ALWAYS_SHARED)return;
        if(bSet)d->shareMode=GSHARED_OBJECT_SHARED;
        else d->shareMode=GSHARED_OBJECT_NOTSHARED;
    }
    inline bool isShared() const {return d->shareMode == GSHARED_OBJECT_SHARED || d->shareMode == GSHARED_OBJECT_ALWAYS_SHARED;}
    inline P *sharedObject(){return d;}
    inline const P *sharedObject() const{return d;}
    inline virtual bool isEmpty() const {return d==0;}

protected:
    virtual void clearPrivate(){
        if(!d)return;
        d->refCount--;
        if(!d->refCount){
            d_factory->remove(d);
        }
        d=0;
    }

};


}

#endif
