#include "impl/gCslPrivate.h"

#define ca gCslPrivate *o=(gCslPrivate *)d
#define co gCslPrivate *io=(gCslPrivate *)other.d
#define cn ca;if(!o){o=new gCslPrivate;d=o;}
#define acn gassert(d,"gCsl() object not initialized")
gCsl::gCsl():gBaseShared(new gCslPrivateFactory){

}

gCsl::gCsl(const gCsl &other):gBaseShared(other,new gCslPrivateFactory){

}
gCsl::~gCsl(){

}

gCsl &gCsl::operator =(const gCsl &other){
    assign(other);
    return *this;
}
bool gCsl::openScript(const gString &fileName){
    cn;
    return o->lexer.openScript(fileName);
}

bool gCsl::setScript(const gString &script){
    cn;
    return o->lexer.setScript(script);
}

bool gCsl::hasErrors() const{
    acn;
    ca;
    return o->errors.isEmpty() == false;
}

gu32 gCsl::getErrorCount() const{
    acn;
    ca;
    return o->errors.size();
}

gCslError *gCsl::getError(gu32 index) const{
    acn;
    ca;
    return o->errors.pValue(index);
}

gTree *gCsl::getParseTree(){
    acn;
    ca;
    return &o->tree;
}

//Main tamale
void gCsl::parse(bool bSingleLine){
    acn;
    ca;
    if(o->lexer.isEmpty())
        return;
    o->parseMain(bSingleLine);
}
gCslNode *gCsl::getFirstNode(){
    acn;
    ca;
    o->tree.gotoMain();
    return o->getChildNode();
}

gCslNode *gCsl::walk(){
    gCslNode *node,*dnode;
    acn;
    ca;
    o->bDone=false;
    dnode=0;
    while(!o->bDone){
        if(o->level<=o->lastLevel)
            o->tree.nextChild();
        else if(o->level > o->lastLevel)
            o->tree.selFirstChild();
        o->lastLevel=o->level;
        node=o->getChildNode();
        if(node){
            o->tree.gotoChild();
            if(o->tree.hasChilds()){
                o->level++;
            }else{
                o->tree.gotoFather();
            }
            o->bDone=true;
        }else{
            o->level--;
            if(o->level){
                o->tree.gotoFather();
            }else{
                o->bDone=true;
            }
        }
    }
    if(o->level){
        o->walkNode=node;
        return node;
    }else{
        o->level=1;
        o->lastLevel=0;
        o->walkNode=0;
        o->tree.gotoMain();
        return 0;
    }
}
// Just aadds a level but
void gCsl::addLevel(const gString &slevel){
    acn;
    ca;
    gCslLevel *l=o->levels.append(slevel);
    if(l){
        l->level=o->level;
        l->node=o->walkNode;
    }

}

void gCsl::setLevel(const gString &slevel){
    acn;
    ca;
    gCslLevel *l=o->levels.append(slevel);
    if(l){
        l->level=o->level;
        l->node=o->walkNode;
    }else{
        l=new gCslLevel();
        l->sid=slevel;
        l->level=o->level;
        l->node=o->walkNode;
        o->levels.append(l);
    }
}
void gCsl::checkLevelDepth(){
    acn;
    ca;
    gCslLevel *l;
    if(!o->walkNode)return;
    for(gu32 i=0;i<o->levels.size();i++){
        l=o->levels.value(i);
        if(l->level >= o->level && o->walkNode->parent!=l->node && l->level!=GCSL_LEVEL_UNASIGNED){
            l->level=GCSL_LEVEL_OUT;
        }
    }
}

bool gCsl::inLevel(const gString &slevel) const{
    acn;
    ca;
    gCslLevel *l=o->levels.search(slevel);
    if(l){
        return o->level >= l->level;
    }
    return false;
}
bool gCsl::outLevel(const gString &slevel) const{
    acn;
    ca;
    gCslLevel *l=o->levels.search(slevel);
    if(l){
        return l->level==GCSL_LEVEL_OUT;
    }
    return false;
}
gCslDefinition *gCsl::addDefinition(const gString &ident, gs32 id, gs32 type){
    acn;
    ca;
    gCslDefinition *def=o->getDefinition(ident);
    if(def)return 0;
    def=new gCslDefinition(ident,id,type);
    o->definitions.append(def);
    return def;
}
gCslDefinition *gCsl::getDefinition(const gString &ident) const{
    acn;
    ca;
    return o->getDefinition(ident);
}
gCslDefinition *gCsl::getDefinition(gu32 index) const{
    acn;
    ca;
    return o->definitions.pValue(index);
}
gu32 gCsl::getDefinitionCount() const{
    acn;
    ca;
    return o->definitions.size();
}
void gCsl::setStartOfBlockCodeToken(gs32 set){
    acn;
    ca;
    o->openBlock=set;
}
void gCsl::setEndOfBlockCodeToken(gs32 set){
    acn;
    ca;
    o->closeBlock=set;
}
void gCsl::setEndOfStatementToken(gs32 set){
    acn;
    ca;
    o->endofStatement=set;
}
