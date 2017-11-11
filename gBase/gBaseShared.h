#ifndef GBASESHARED_H
#define GBASESHARED_H
#include "gObjectSharingModel.h"
namespace gfw{

class SHARED_GFW gBaseShared:public gObjectSharingModel<gSharedObject>{
public:
    gBaseShared(gSharedObjectFactory *_factory):
        gObjectSharingModel<gSharedObject>(_factory){

    }
    gBaseShared(const gBaseShared &other,gSharedObjectFactory *_factory):
        gObjectSharingModel<gSharedObject>(other,_factory){

    }

    ~gBaseShared(){

    }

};

}


#endif // GBASESHARED_H
