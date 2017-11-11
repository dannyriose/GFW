#ifndef GSHAREDOBJECT_H
#define GSHAREDOBJECT_H
#include "gtypes.h"
#include "gTemplates.h"
#include "gPrivateFactory.h"
#include "gassert.h"
#include "gfw_export.h"
namespace gfw{
    //! Base class for Objects that use shared data. Mostly private members
   enum GSHARED_OBJECT_MODE{
       GSHARED_OBJECT_NOTSHARED,
       GSHARED_OBJECT_SHARED,
       GSHARED_OBJECT_ALWAYS_SHARED
   };

   struct gSharedObject{

       //! Reference count
       mutable gs32 refCount;
       //! Flag that determines whether the shares data or not
       mutable gs32 shareMode;
       //! Protected member used on shared objects 
       gSharedObject():refCount(1),shareMode(GSHARED_OBJECT_NOTSHARED){

       }
       gSharedObject(GSHARED_OBJECT_MODE _shared):refCount(1),shareMode(_shared){

       }

       gSharedObject(const gSharedObject *other):refCount(1),shareMode(GSHARED_OBJECT_NOTSHARED){
           copy(other);
       }

       virtual ~gSharedObject(){

       }
       virtual void copy(const gSharedObject *other){
           (void)other;
       }
       template <class V>
       V *upcast() {return static_cast<V*>(this);}
   };




}
#endif // GSHAREDOBJECT_H
