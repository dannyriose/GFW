#include "impl/gUrlPrivate.h"
#define acn gassert(d,"URL NOT SET");
gUrl::gUrl():gBaseShared(new gUrlPrivateFactory){

}

gUrl::gUrl(const gUrl &other):gBaseShared(other,new gUrlPrivateFactory){

}

gUrl::gUrl(const gString &url): gBaseShared(new gUrlPrivateFactory){
    gUrlPrivate *o = new gUrlPrivate;

    o->fullurl = url;
    o->parseUrl();
    d = o;

}
gUrl::~gUrl(){

}

gUrl &gUrl::operator =(const gUrl &other){
    assign(other);
    return *this;
}

void gUrl::setUrl(const gString &url){
    ca;
   if(!d){
       o = new gUrlPrivate;
       d = o;
   }

   o->fullurl = url;
   o->parseUrl();
}


const gString &gUrl::host() const{
    acn;
    ca;
    return o->host;
}


const gString &gUrl::domain() const{
    acn;
    ca;
    return o->domain;
}
const gString &gUrl::subDomain(gu32 index) const{
    acn;
    ca;
    if(index >= o->subdomains.size())
        return o->empty;
    return o->subdomains.value(index);
}
gu32 gUrl::subDomainCount() const{
    acn;
    ca;
    return o->subdomains.size();
}
const gString &gUrl::path(gu32 index) const{
    acn;
    ca;
    if(index >= o->path.size())
        return o->empty;
    return o->path.value(index);
}
gu32 gUrl::pathCount() const{
    acn;
    ca;
    return o->path.size();
}
bool gUrl::containsPath(const gString &name, gu32 *indexOut) const{
    acn;
    ca;
    return o->path.contains(name,indexOut);
}
const gString &gUrl::absPath() const{
    acn;
    ca;
    return o->abspath;
}

const gString &gUrl::query(const gString &key) const{
    acn;
    ca;
    if(!o->query.contains(key))
        return o->empty;
    return o->query.value(key);

}
bool gUrl::containsQuery(const gString &key) const{
    acn;
    ca;
    return o->query.contains(key);
}
const gString &gUrl::fragment() const{
    acn;
    ca;
    return o->fragment;
}
const gString &gUrl::url() const{
    acn;
    ca;
    return o->fullurl;
}

const gString &gUrl::scheme() const{
    acn;
    ca;
    return o->scheme;
}

const gString &gUrl::user() const{
    acn;
    ca;
    return o->user;
}
const gString &gUrl::password() const{
    acn;
    ca;
    return o->password;
}
gs32 gUrl::port() const{
    acn;
    ca;
    if(o->port.isInteger()){
        return o->port.toInt();
    }
    return -1;
}
