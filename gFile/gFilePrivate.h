#ifndef GFILEPRIVATE_H
#define GFILEPRIVATE_H
#include "gFile.h"
#include <sys/types.h>
#include<sys/stat.h>
#ifdef WIN32
#include <io.h>
#include <Windows.h>
#else
#include <unistd.h>
#include <sys/io.h>
#include <sys/types.h>
#include <pwd.h>
#endif


namespace gfw{
struct gFilePrivate:public gSharedObject
{
    FILE *f; //the file
    gu64 size; //the file size
    gString fname;
    //Object always shared
    gFilePrivate(){

    }

    gFilePrivate(FILE *_f,gu64 _size, const gString &_fname):gSharedObject(GSHARED_OBJECT_ALWAYS_SHARED),
        f(_f),
        size(_size),
        fname(_fname)
    {

    }
    ~gFilePrivate(){
        fclose(f);
    }

};
G_SHAREDOBJECT_FACTORY(gFilePrivateFactory,gFilePrivate);
}
#endif // GFILEPRIVATE_H
