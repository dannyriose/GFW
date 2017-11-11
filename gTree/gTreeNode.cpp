#include "impl/gTreeNodePrivate.h"
using namespace gfw;

#define ca GFW_PRIVATE_CAST(o,d,gTreeNodePrivate)
#define co GFW_PRIVATE_CAST(io,other.d,gTreeNodePrivate)
#define acn gassert(d && this,"gTree object is empty")
#define cn if(!d)return

gTreeNode::gTreeNode():gBaseShared(new gTreeNodePrivateFactory(this)){
}

gTreeNode::gTreeNode(const gTreeNode &other):gBaseShared(other,new gTreeNodePrivateFactory(this))
{

}


void gTreeNode::create(const void *data, gTreeNode *father, gTree *tree, IClean func, gu32 index)
{
    clear();

    gTreeNodePrivate *o=new gTreeNodePrivate(this);d=o;
    o->userdata=(void *)data;
    o->func=func;
    o->father=father;
    o->childs.create();
    o->childs.setFunction(childClean);
    o->index=index;
    o->tree=tree;
}

gTreeNode::~gTreeNode()
{
}


gTreeNode *gTreeNode::addChild(const void *data, gfw::IClean func)
{
    acn;

    ca;
    gTreeNode *node=new gTreeNode();
    o->childs.append(node);
    o->childs.last();
     node->create(data,this,o->tree,func,o->childs.index());
    return node;
}

void gTreeNode::removeChild()
{
    acn;
    ca;
    o->childs.remove();
}

void gTreeNode::removeChild(gu32 index)
{
    acn;
    ca;
    o->childs.remove(index);
}

gu32 gTreeNode::childCount() const
{
    acn;
    ca;
    return o->childs.count();
}


gTreeNode *gTreeNode::firstChild()
{
    acn;
    ca;
    o->childs.first();
    return (gTreeNode *)o->childs.data();
}

gTreeNode *gTreeNode::lastChild()
{
    acn;
    ca;
    o->childs.last();
    return (gTreeNode *)o->childs.data();
}

gTreeNode *gTreeNode::child() const
{
    acn;
    ca;
    return (gTreeNode *)o->childs.data();
}


gTreeNode *gTreeNode::child(gu32 index)
{
    acn;
    ca;
    o->childs.select(index);
    return (gTreeNode *)o->childs.data();
}

gTreeNode *gTreeNode::father() const
{
    acn;
    ca;
    return o->father;
}
gTreeNode *gTreeNode::nextChild()
{
    acn;
    ca;
    gTreeNode *ret;
    ret=(gTreeNode *)o->childs.data();

    o->childs.next();
    return ret;
}

gTree *gTreeNode::owner() const{
    acn;
    ca;
    return o->tree;
}
void gTreeNode::clearChild(){
    acn;
    ca;

    o->childs.clear();
}
void gTreeNode::clearUserData(){
    acn;
    ca;
    if(o->func)
        o->func(o->userdata);
}




gTreeNode &gTreeNode::operator =(const gTreeNode &other)
{
   assign(other);
   return *this;
}

void gTreeNode::childClean(void *data)
{
    gTreeNode *n=(gTreeNode *)data;

    if(n){
        delete n;
    }
}

void gTreeNode::setFather(gTreeNode *father)
{
    acn;
    ca;

    o->father=father;
}

void *gTreeNode::userdata() const
{
    acn;
    ca;
    return o->userdata;
}

void gTreeNode::setUserData(const void *data)
{
    acn;
    ca;
    o->userdata=(void *)data;
}

int gTreeNode::nodeIndex() const
{
    cn -1;
    ca;

    return o->index;
}

int gTreeNode::childIndex() const
{
    cn -1;
    ca;
    return o->childs.index();
}

IClean gTreeNode::userFunction() const
{
    cn 0;
    ca;

    return o->func;
}


bool gTreeNode::hasChilds() const
{
    cn false;
    ca;
    return o->childs.isEmpty() == false;

}

bool gTreeNode::childIsNull() const
{
    cn false;
    ca;
    return o->childs.data()==0;
}
