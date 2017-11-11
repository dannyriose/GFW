#ifdef __gnu_linux__
#include "gLibrary.h"
#include <dlfcn.h>
using namespace gfw;
struct glibrary_data_unix{
    void *handle;
    gString path;
    glibrary_data_unix():handle(0){

    }
};


#define ca glibrary_data_unix *o=(glibrary_data_unix *)d

gLibrary::gLibrary(){
    d=new glibrary_data_unix;
}

gLibrary::~gLibrary(){
    ca;
    delete o;
}

bool gLibrary::load(const gString &file){
    ca;
    o->handle=dlopen(file.ascii(),RTLD_LAZY);
    return o->handle!=0;
}

void *gLibrary::resolve(const gString &func) const{
    ca;
    if(!o->handle)return 0;
    return dlsym(o->handle,func.ascii());
}
bool gLibrary::isLoaded() const{
    ca;
    return o->handle!=0;
}
const gString &gLibrary::path() const{
    ca;
    return o->path;
}
void gLibrary::unload(){
    ca;
    if(o->handle){
        dlclose(o->handle);
    }
}

#endif
