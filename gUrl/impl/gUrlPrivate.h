#ifndef GURLPRIVATE_H
#define GURLPRIVATE_H
#include <gUrl/gUrl.h>
#include <gLexer/gLexer.h>
#include <gString/gStringList.h>
#include <gHashMap/gHashMap.h>
using namespace gfw;

struct gUrlPrivate:public gSharedObject{
    gString fullurl;
    gString host;
    gString abspath;
    gString scheme;
    gHashMap<gString,gString> query;
    gString fragment;
    gString user;
    gString password;
    gString tld;
    gString domain;
    gString port;
    gString file;
    gStringList subdomains;
    gStringList path;
    bool error;
    gString empty; //to hold empty return values
    gUrlPrivate():error(false){

    }
    ~gUrlPrivate(){

    }

    virtual void copy(const gSharedObject *other){
        const gUrlPrivate *o = static_cast<const gUrlPrivate *>(other);
        fullurl = o->fullurl;
        host = o->host;
        abspath = o->abspath;
        path = o->path;
        scheme = o->scheme;
        query = o->query;
        fragment = o->fragment;
        user = o->user;
        password = o->password;
        tld = o->tld;
        domain = o->domain;
        port = o->port;
        subdomains = o->subdomains;
        file = o->file;
    }
    //main function

    void clear(){
        host.clear();
        abspath.clear();
        path.clear();
        scheme.clear();
        query.clear();
        fragment.clear();
        user.clear();
        password.clear();
        tld.clear();
        domain.clear();
        port.clear();
        subdomains.clear();
        file.clear();
    }

    void parseUrl(){
        gLexer l;
        l.setScript(fullurl);
        clear();
        //this is the format
        //scheme://user:password@sub.domain.com:2021/path/filename.html#fragment?query=falues
        if(!parseScheme(l)){
            //! if no scheme found then go back and reset
            l.reset();
        }
        gu32 lastposition = l.position();
        //Next guess would be user and password
        if(!parseUserPassword(l)){
            l.setPosition(lastposition);
        }
        lastposition = l.position();
        //Nest guess would be domain and subdomain and port
        if(!parseDomains(l)){
            l.setPosition(lastposition);
        }
        parsePath(l);
        //Next part would be the path which includes...the path,file, fragment and query
    }
    /*******************************************************************************/
    //! Parse the scheme of an url
    bool parseScheme(gLexer &l){
        gString s;
        bool found = false;
        int state = 15;
        int diagcount = 0;
        l.getToken();
        if(l.tokenID1() == TOKEN_TYPE_IDENT){
            s= l.tokenString();
            state = 0;
        }
        while(state!=15){
            if(diagcount < 2)
                l.getToken();
            switch(state){
            case 0:
                if(l.tokenID1() == TOKEN_TYPE_TWODOTS)
                    state = 1;
                else
                    state = 15;
                break;
            case 1:
               if(l.tokenID1() == TOKEN_TYPE_DIAGONAL && diagcount < 2)
                   diagcount++;
               else
                   state = 15;
               if(diagcount == 2)
               {
                   state = 15;
                   found = true;
               }
               break;
            }
        }
        if(found){
            scheme = s;
        }
        return found;
    }
    /***************************************************************************************/
    //! Parse user and password if any
    bool parseUserPassword(gLexer &l){
        //expected two  idents separated by two dots.
        int state = 0;
        gString u,p;
        while(state!=15){
            l.getToken();
            switch(state){
            case 0:
                if(l.tokenID1() == TOKEN_TYPE_IDENT){
                    if(u.isEmpty()){
                        u = l.tokenString();
                        state = 1;
                    }
                    else if(p.isEmpty()){
                        p = l.tokenString();
                        state = 3;
                    }
                    else{
                        state = 15;
                    }
                }
                else{
                    state = 15;
                }
                break;
            case 1:
                if(l.tokenID1() == TOKEN_TYPE_TWODOTS)
                    state = 0;
                else
                    state = 15;
                break;
            case 3:
                if(l.tokenID1() != TOKEN_TYPE_ARROBA){

                    u.clear(); p.clear();
                }
                state = 15;
                break;
            }
        }
        if(!u.isEmpty() && !p.isEmpty()){
            user = u; password = p;
            return true;
        }
        return false;
    }
    /****************************************Parse the domain and subdomains*********************/
    bool parseDomains(gLexer &l){
        gStringList domains;
        gString _port;
        int state = 0;

        //the mechanics is going from ident dot ident dot until we get something else
        while(state!=15){
            l.getToken();
            switch(state){
            case 0: //  ident
                if(l.tokenID1() == TOKEN_TYPE_IDENT){
                    domains.append(l.tokenString());
                    state = 1;
                }
                else{
                    state = 15;
                }
                break;
            case 1:
                if(l.tokenID1() == TOKEN_TYPE_DOT)
                    state = 0;
                else if(l.tokenID1() == TOKEN_TYPE_TWODOTS){ // a port
                    state = 2;
                }
                else
                    state = 15;
                break;
            case 2:
                if(l.tokenID1() == TOKEN_TYPE_NUMINT){
                    _port = l.tokenString();
                }
                //expected to be the end.
                state = 15;
                break;
            }
        }
        //our tld must be
        if(!domains.isEmpty()){
            if(domains.size() == 1){
                tld = domains.value(0);
                host = tld;
            }
            else{
                tld = domains.value(domains.size() - 1);
                domains.remove(domains.size() - 1,1);
                domain = domains.value(domains.size() - 1) +  gString(".")+tld;
                domains.remove(domains.size() - 1,1);
                if(!domains.isEmpty()){
                  subdomains = domains;
                }
                //build the host
                for(gu32 i = 0; i<subdomains.size(); i++){
                    host += subdomains[i] + gString(".");
                }
                host+=domain;
            }
            //the last token we found we must take it one stepback so the next function could retrieve it
            if(_port.isEmpty())
            {
                l.stepBack();
            }
            else{
                port = _port;
            }
            return true;
        }
        return false;
    }
    //***********************************************/
    bool parsePath(gLexer &l){
        //the mechancs is dash/ident/dash/ident dot ident until we get end of file or fragment or query
        gStringList _path;
        gString _abspath;
        gString _fragment;
        gString dot;
        gList<gStringPair> _query;
        bool processingFragment = false;
        bool processingQuery  = false;
        int state = 0;

        while(state != 15){
            l.getToken();
            switch(state){
            case 0: //Diag
                switch(l.tokenID1()){
                case TOKEN_TYPE_DIAGONAL:
                    state = 1;
                    break;
                case TOKEN_TYPE_DOT:
                case TOKEN_TYPE_GUIONNORMAL:
                    state = 1;
                    dot = l.tokenString();
                    break;
                case TOKEN_TYPE_CAT: //fregment
                    if(!processingFragment){
                        state = 2;
                        processingFragment = true;
                    }
                    else{
                        state = 15;
                    }

                    break;
                case TOKEN_TYPE_QUESTIONCLOSE://query
                    if(!processingQuery){
                        state = 3;
                        processingQuery = true;
                    }
                    else{
                        state = 15;
                    }
                    break;
                case TOKEN_TYPE_ANDSIMBOL:
                    if(processingQuery)
                        state = 3;
                    else
                        state = 15;
                    break;
                case TOKEN_TYPE_EQUAL:
                    if(processingQuery)
                        state = 4;
                    else
                        state = 15;
                    break;
                default:
                    state = 15;
                }
                if(state!=15)
                {
                    _abspath+=l.tokenString();
                }
                break;
            case 1://iident
                switch(l.tokenID1()){
                case TOKEN_TYPE_IDENT:
                case TOKEN_TYPE_NUMINT:
                case TOKEN_TYPE_NUMFLOAT:
                case TOKEN_TYPE_DOT:
                    _abspath+=l.tokenString();
                    if(!dot.isEmpty() && !_path.isEmpty()){
                        gString &v = _path.value(_path.size() - 1);
                        v+=dot + l.tokenString();
                        dot.clear();
                    }
                    else
                    {
                        _path.append(l.tokenString());
                    }
                    state = 0;
                    break;
                default:
                    state = 15;
                }
                break;
            case 2: //fragment
                if(l.tokenID1() == TOKEN_TYPE_IDENT){
                    _fragment = l.tokenString();
                    state = 0;
                }else{
                    state = 15;
                    //error
                    error = true;
                }
                break;
            case 3: //query ident
                if(l.tokenID1() == TOKEN_TYPE_IDENT){
                    _query.append(new gStringPair(l.tokenString(),gString()));
                    state = 0;
                }
                else{
                    state = 15;
                }
                break;
            case 4: //query value
                switch(l.tokenID1()){
                case TOKEN_TYPE_IDENT:
                case TOKEN_TYPE_NUMFLOAT:
                case TOKEN_TYPE_NUMINT:
                    _query.value(_query.size() -1).t2 = l.tokenString();
                    state = 0;
                default:
                    state = 15;
                }
            }
        }
        if(_abspath.isEmpty()){
            return false;
        }
        abspath = _abspath;
        if(!_query.isEmpty()){
            query.alloc(_query.size());
            for(gu32 i = 0; i <_query.size();i++){
                gStringPair &p = _query.value(i);
                query.set(p.t1,p.t2);
            }
        }
        fragment = _fragment;
        path = _path;
        return true;
    }
};
G_SHAREDOBJECT_FACTORY(gUrlPrivateFactory,gUrlPrivate);
#define ca gUrlPrivate *o = static_cast<gUrlPrivate *>(d)
#endif // GURLPRIVATE_H
