#ifndef GSTRINGPRIVATE_H
#define GSTRINGPRIVATE_H
#include <gString/gString.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <wctype.h>
#include <gString/gStringList.h>
namespace gfw{
    struct gStringPrivate:public gSharedObject
    {

       char *data;//most basic data
       gu32 size;//size or character count
       gu32 sizebytes;//size on bytes
       int format;//format ascii or widestring
       gStringPrivate():data(0),
           size(0),
           sizebytes(0),
           format(0)
       {

       }
       void clear(){
           if(!data)return;
           free(data);
           size=0;
           sizebytes=0;
       }

       ~gStringPrivate(){
            clear();
       }
       bool alloc(gu32 _size, gs32 _format){
           gu32 totalSize,asize;
           //gassert(_size,"_size can't be 0");
           if(_size==0)return false;
           clear();
           switch(_format)
           {
           case gString::ASCII:
               totalSize=_size;
               asize=_size + 1;
               break;
           case gString::UTF:
               totalSize=(_size) * sizeof(wchar_t);
               asize=totalSize + sizeof(wchar_t);
               break;
           }


           format=_format;
           size=_size;
           sizebytes=totalSize;
           data=(char *)malloc(asize);
           if(!data){
               return false;
           }
           memset(&data[totalSize],0,asize - totalSize);//set null string activated
           return true;
       }

       virtual void copy(const gSharedObject *other){
           GFW_PRIVATE_CAST(o,other,gStringPrivate);
           if(!o->data)return;
           if(alloc(o->size,o->format)){
               memcpy(data,o->data,o->sizebytes);
           }

       }
    };
    G_SHAREDOBJECT_FACTORY(gStringPrivateFactory,gStringPrivate);
}
#endif // GSTRINGPRIVATE_H
