#include "gCsl.h"

using namespace gfw;



gCslNode::gCslNode():id(-1),type(GCSL_NODE_UNSET),
    member(0),
    property(0),
    parent(0),
    assignto(0),
    assign(0),
    bHasChild(false){

}
gCslNode::gCslNode(const gString &_ident, gCslNode *_parent, gs32 _id, gs32 _type):ident(_ident),
    id(_id),
    type(_type),
    member(0),
    property(0),
    assignto(0),
    assign(0),
    parent(_parent),
    bHasChild(false){

}

gCslNode::~gCslNode(){
    if(member)
        delete member;
    if(property)
        delete property;
    if(assign)
        delete assign;
    if(assignto)
        delete assignto;
}

bool gCslNode::hasParam(const gString &_ident) const{
    gCslNode *par;
    for(gu32 i=0;i<params.size();i++){
        par=params.pValue(i);
        if(par->ident==_ident)return true;
    }
    return false;
}

gCslNode *gCslNode::addParam(const gString &_ident,gs32 _id, GCSL_NODE_TYPE _type){
    gCslNode *par=new gCslNode(_ident,0,_id,_type);
    params.append(par);
    return par;
}

gCslNode *gCslNode::getParam(gu32 index){
   return params.pValue(index);
}

gCslNode *gCslNode::getParam(const gString &_ident){
    gCslNode *par;
    for(gu32 i=0;i<params.size();i++){
        par=params.pValue(i);
        if(par->ident==_ident)return par;
    }
    return 0;
}
gu32 gCslNode::getParamCount() const{
    return params.size();
}
void gCslNode::setMember(const gString &_ident, gs32 _id, GCSL_NODE_TYPE _type){
    member=new gCslNode(_ident,this,_id,_type);
}
void gCslNode::setProperty(const gString &_ident, gs32 _id, GCSL_NODE_TYPE _type){
    property=new gCslNode(_ident,this,_id,_type);
}
void gCslNode::setAssign(const gString &_ident, gs32 _id, GCSL_NODE_TYPE _type){
    assign=new gCslNode(_ident,this,_id,_type);
}
void gCslNode::setAssignTo(const gString &_ident, gs32 _id, GCSL_NODE_TYPE _type){
    assignto=new gCslNode(_ident,this,_id,_type);
}
