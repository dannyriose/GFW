#ifndef GSCODEPRIVATE_H
#define GSCODEPRIVATE_H
#include "gsCoder.h"
#include <gMemory/gBaseArray.h>
#include <time.h>
#include <math.h>
#include <memory.h>
#include <cstdlib>

namespace gfw{
struct gsCodePrivate:public gSharedObject{
    gString key;//the key
    gu16 **table;
    gu32 width;
    gu32 height;

    gsCodePrivate():table(0),width(0),height(0){
    }

    bool alloc(int w,int h){
        table=new gu16*[w];
        if(table){
            for(int i=0;i<w;i++){
                table[i]=new gu16[h];
            }
            width=w;
            height=h;
        }
        return table!=0;
    }

    void clear(){
        if(table)
        {
            for(gu32 i=0;i<width;i++)
            {
                delete [] table[i];
            }
            delete [] table;
            table=0;
        }
    }
    ~gsCodePrivate()
    {
        clear();
    }
    virtual void copy(const gSharedObject *other){
        GFW_PRIVATE_CAST(o,other,gsCodePrivate);
        key=o->key;
        if(alloc(o->width,o->height)){
            for(gu32 i=0;i<width;i++){
                memcpy(table[i],o->table[i],height * sizeof(gu16));
            }
        }
    }
};
G_SHAREDOBJECT_FACTORY(gsCodePrivateFactory,gsCodePrivate);
}

#endif // GSCODEPRIVATE_H
