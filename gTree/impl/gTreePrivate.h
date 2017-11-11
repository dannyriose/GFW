#ifndef GTREEPRIVATE_H
#define GTREEPRIVATE_H
#include <gTree/gTree.h>


namespace gfw{
struct gTreePrivate:public gSharedObject
{
    gTreeNode main;
    gTreeNode *node;
    gTree *thistree;
    gTreePrivate(gTree *_thistree):node(0),thistree(_thistree){

    }
    ~gTreePrivate(){

    }

    virtual void copy(const gSharedObject *other){
        GFW_PRIVATE_CAST(o,other,gTreePrivate);
        //delete the entire tree
        main=o->main;
        node=&main;
    }

    static void copyNodes(gTree *source, gTree *dest)
    {
        gTreeNode *n,*c;

        n=source->node();

        while((c=n->nextChild())!=0)
        {
            dest->addNode(c->userdata(),c->userFunction());
            source->gotoChild();
            dest->gotoChild();
            copyNodes(source,dest);
            source->gotoFather();
            dest->gotoFather();
        }

    }

};
struct gTreePrivateFactory:public gSharedObjectFactory{
    gTree *d;
    gTreePrivateFactory(gTree *_d):d(_d){

    }
    ~gTreePrivateFactory(){

    }
    virtual gSharedObject *create() const{
        return new gTreePrivate(d);
    }
    virtual void remove(gSharedObject *obj) const{
        delete obj;
    }
    virtual void deleteme(){
        delete this;
    }
};
}
#endif // GTREEPRIVATE_H
