
#ifdef WIN32
#include "gLibrary.h"
#include <Windows.h>
#include <gBase/gassert.h>
using namespace gfw;
struct library_data
{
    HMODULE lib;
    gString path;
    library_data():lib(0)
    {

    }
    ~library_data()
    {
        if(lib)
            FreeLibrary(lib);
    }

};
#define ca library_data *o=(library_data *)d
gLibrary::gLibrary()
{
    d=new library_data;
}

gLibrary::~gLibrary()
{
    ca;
    delete o;
}

bool gLibrary::load(const gString &file)
{
    ca;
    o->path=file;
#ifdef UNICODE
    o->path.thisToUnicode();
    o->lib=LoadLibrary(o->path.unicode());
#else
    o->lib=LoadLibrary(file.ascii());
#endif
    return o->lib!=NULL;
}

void gLibrary::unload()
{
    ca;
    if(o->lib)
    {
        FreeLibrary(o->lib);
        o->lib=0;
        o->path.clear();
    }
}

void *gLibrary::resolve(const gString &func) const
{
    ca;
    gassert(o->lib,"Library not loaded");
    return GetProcAddress(o->lib,func.ascii());

}

bool gLibrary::isLoaded() const{
    ca;
    return o->lib!=0;
}
const gString &gLibrary::path() const{
    ca;
    return o->path;
}
#endif
