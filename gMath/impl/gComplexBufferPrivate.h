#ifndef GCOMPLEXBUFFERPRIVATE_H
#define GCOMPLEXBUFFERPRIVATE_H
#include <gMath/gComplexBuffer.h>

namespace gfw{
struct gComplexBufferPrivate:public gSharedObject
{
    gBaseArray<gComplexNumber> data;
    gComplexBufferPrivate(){

    }
    ~gComplexBufferPrivate(){

    }
    virtual void copy(const gSharedObject *other){
        GFW_PRIVATE_CAST(o,other,gComplexBufferPrivate);
        data=o->data;
    }
};
G_SHAREDOBJECT_FACTORY(gComplexBufferPrivateFactory,gComplexBufferPrivate);
}
#endif // GCOMPLEXBUFFERPRIVATE_H
