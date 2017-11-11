#ifndef GTREENNODEPRIVATE_H
#define GTREENNODEPRIVATE_H
#include <gTree/gTree.h>
#include <gLinkedList/gLinkedList.h>
namespace gfw{
struct gTreeNodePrivate:public gSharedObject
{
    gLinkedList childs;
    IClean func;
    void *userdata;
    int index;
    gTreeNode *thisnode;
    gTreeNode *father;
    gTree *tree;
    gTreeNodePrivate(gTreeNode *_thisnode):
        func(0),
        userdata(0),
        index(0),
        thisnode(_thisnode),
        father(0),
        tree(0)
    {

    }
    gTreeNodePrivate(const gTreeNodePrivate &other,gTreeNode *_thisnode):
        childs(other.childs),
        func(other.func),
        userdata(other.userdata),
        index(other.index),
        thisnode(_thisnode),
        father(other.father),
        tree(other.tree)
    {

    }
    ~gTreeNodePrivate(){
        if(func)
            func(userdata);
    }
    void clear(){
        childs.clear();
        if(func)func(userdata);
    }

    static void copyNodes(gTreeNodePrivate *dest,const gTreeNodePrivate *other,gTree *_owner){
        gTreeNode *n,*no;
        gTreeNodePrivate *op,*nop;
        other->childs.first();
        dest->childs.last();
        while((n=(gTreeNode *)other->childs.data())!=0){
            no=new gTreeNode();
            op=static_cast<gTreeNodePrivate *>(n->sharedObject());
            no->create(op->userdata,dest->thisnode,_owner,op->func,other->childs.index());
            if(n->hasChilds()){
                nop=static_cast<gTreeNodePrivate *>(no->sharedObject());
                copyNodes(nop,op,_owner);
            }
            dest->childs.append(no);
            other->childs.next();
        }

    }

    virtual void copy(const gSharedObject *other){
        GFW_PRIVATE_CAST(o,other,gTreeNodePrivate);
        clear();
        userdata=o->userdata;
        index=o->index;
        func=o->func;
        copyNodes(this,o,tree);
    }

};
struct gTreeNodePrivateFactory:public gSharedObjectFactory{
    gTreeNode *d;
    gTreeNodePrivateFactory(gTreeNode *_d):d(_d){

    }
    ~gTreeNodePrivateFactory(){

    }
    virtual gSharedObject *create() const{
        return new gTreeNodePrivate(d);
    }
    virtual void remove(gSharedObject *obj) const{
        delete obj;
    }
    virtual void deleteme(){
        delete this;
    }
};
}
#endif // GTREENNODEPRIVATE_H
