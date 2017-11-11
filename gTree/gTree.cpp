#include "impl/gTreePrivate.h"
using namespace gfw;


#define ca gTreePrivate *o=(gTreePrivate *)d
#define co gTreePrivate *io=(gTreePrivate *)other.d()
#define cn if(!d) return
#define acn gassert(d,"Object is empty")
gTree::gTree():gBaseShared(new gTreePrivateFactory(this)){

}

gTree::gTree(const gTree &other):gBaseShared(other,new gTreePrivateFactory(this)){

}

gTree::~gTree(){

}


bool gTree::create(const void *data, IClean func)
{
    clear();
    gTreePrivate *o=new gTreePrivate(this);

    if(o)
    {
        o->main.create(data,0,this,func,0);
        o->node=&o->main;
        d=o;
    }
    return d!=0;

}

void gTree::addNode(const void *data, IClean func)
{
    acn;
    ca;
    gassert(o->node,"gTreeNode node is null");
   o->node->addChild(data,func);

}

gTreeNode *gTree::gotoMain()
{
    acn;
    ca;
    o->node=&o->main;
    return o->node;
}


gTreeNode *gTree::selFirstChild()
{
    acn;
    ca;
    return o->node->firstChild();
}

gTreeNode *gTree::selLastChild()
{
    acn;
    ca;

    return o->node->lastChild();
}

gTreeNode *gTree::selChild(gu32 index)
{
    acn;
    ca;
    return o->node->child(index);
}

gTreeNode *gTree::nextChild()
{
    acn;
    ca;
    return o->node->nextChild();
}

gu32 gTree::childCount() const
{
    cn 0;
    ca;
    return o->node->childCount();
}

gTreeNode *gTree::gotoChild()
{
    acn;
    ca;

    o->node=o->node->child();
    return o->node;
}

gTreeNode *gTree::gotoFather()
{
    acn;
    ca;
    o->node=o->node->father();
    return o->node;
}

gTreeNode *gTree::node() const
{
    acn;
    ca;
    return o->node;
}
void *gTree::nodeData() const
{
    acn;
    ca;
    return o->node->userdata();
}

void *gTree::childData() const
{
    acn;
    ca;
    if(!o->node->hasChilds())return 0;
    if(o->node->childIsNull())return 0;
    gTreeNode *tn=o->node->child();
    if(tn)
        return tn->userdata();
    else
        return 0;
}

int gTree::childIndex() const
{
    acn;
    ca;
    return o->node->childIndex();
}
void *gTree::fatherData() const
{
    acn;
    ca;
    return o->node->father()->userdata();
}

void *gTree::mainData() const
{
    acn;
    ca;
    return o->main.userdata();
}

void gTree::deleteNode()
{
    acn;
    ca;

    gTreeNode *temp;
    int index=o->node->nodeIndex();
    gassert(index!=-1,"Node index can't be -1");
    temp=o->node;

    if(temp==&o->main)
        temp->clear();
    else
    {
        o->node=o->node->father();//goto the father
        o->node->removeChild(index);
    }
    //kill the node
}

void gTree::deleteChildNode()
{
    acn;
    ca;

    o->node->removeChild();
}

void gTree::setData(const void *data)
{
    acn;
    ca;
    o->node->setUserData(data);
}

void gTree::copyFrom(gTree &other)
{
    clear();
    gTreeNode *node;
    if(other.isEmpty())return;

    node=other.gotoMain();//Main

    create(node->userdata(),node->userFunction());

    gTreePrivate::copyNodes(this,&other);
}


gTree &gTree::operator =(const gTree &other)
{
    assign(other);
    return *this;
}

bool gTree::hasChilds() const
{
    cn false;
    ca;
    return o->node->hasChilds();
}

bool gTree::childIsNull() const
{
    cn false;
    ca;
    return o->node->childIsNull();
}

