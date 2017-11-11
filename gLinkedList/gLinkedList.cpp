#include "gLinkedList.h"
#include "linkedl.h"

using namespace gfw;
struct lldata: public CLinkedList
{
    LINKEDLIST *data;
    void *owner;
    int refCount;
    IClean func;
    bool bypass;
    virtual void DeleteElementData(void *d)
    {
        if(func)
            func(d);
    }
    lldata():data(0),owner(0),refCount(0),func(0),bypass(false)
    {
        data=CreateList();
    }
    ~lldata()
    {
        DeleteList(&data);
    }

};
#define ca lldata *o=(lldata *)d
#define cb if(!d) return
gLinkedList::gLinkedList()
{
    d=0;
}

gLinkedList::gLinkedList(const gLinkedList &other)
{
    if(other.isByPass())
    {
        d=other.h();
    }
    else
    {
        *this=other;
    }
}

gLinkedList::~gLinkedList()
{
    clear();
}

void gLinkedList::clear()
{
    cb;
    ca;
    if(o->owner==this)
    {
        o->DeleteList(&o->data);
        delete o;
    }
    d=0;
}

void gLinkedList::create()
{
    if(d)
        clear();
    lldata *o=new lldata;
    o->owner=this;
    d=o;
}

void gLinkedList::append(const gLinkedList &other)
{
    void *data;

    if(other.isEmpty())return;
    other.first();
    while((data=other.data())!=0)
    {
        append(data);
        other.next();
    }
}

void gLinkedList::append(const void *pData)
{
    lldata *o;
    if(!d)
    {
        o=new lldata;
        d=o;
        o->owner=this;
    }
    else
        o=(lldata *)d;
    o->AddElement(o->data,(void *)pData);
}

gu32 gLinkedList::count() const
{
    cb 0;
    ca;
    if(o->IsEmpty(o->data))
        return 0;
    return o->GetNumOfElements(o->data);
}
gu32 gLinkedList::count(const void *pData, gUIntArray *indexesOut, G_SEARCH_OPERATOR op) const{
    cb 0;
    ca;
    if(o->IsEmpty(o->data))return 0;
    void *v;
    bool bOp;
    gu32 c=0;
    if(indexesOut)indexesOut->clear();
    o->SetFirstElement(o->data);
    while((v=o->GetCurrElementData(o->data))!=0){
        G_OP_EQ(op,pData,v,bOp);
        if(bOp){
            if(indexesOut)indexesOut->insert(c);
            c++;
        }
        o->ForwardElement(o->data);
    }
    return c;
}
bool gLinkedList::search(const void *pData, gu32 startPos, gu32 *indexOut, G_SEARCH_OPERATOR op) const{
    cb 0;
    ca;
    if(o->IsEmpty(o->data))return false;
    void *v;
    bool bOp;
    if(startPos==0)
        o->SetFirstElement(o->data);
    else
        o->SetElement(o->data,startPos);
    while((v=o->GetCurrElementData(o->data))!=0){
        G_OP_EQ(op,pData,v,bOp);
        if(bOp){
            if(indexOut)*indexOut=o->GetCurrentIndex(o->data);
            return true;
        }
        o->ForwardElement(o->data);
    }
    return false;
}
bool gLinkedList::contains(const void *pData) const{
    return search(pData);
}
void *gLinkedList::data() const
{
    cb 0;
    ca;
    return o->GetCurrElementData(o->data);
}

void *gLinkedList::data(gu32 index) const
{
    cb 0;
    ca;

    return o->GetElementData(o->data,index);
}

void gLinkedList::first() const
{
    cb;
    ca;

    o->SetFirstElement(o->data);
}

void gLinkedList::next() const
{
    cb;
    ca;
    o->ForwardElement(o->data);
}

void gLinkedList::last() const
{
    cb;
    ca;
    o->SetLastElement(o->data);
}

void *gLinkedList::h() const
{
    return d;
}

void gLinkedList::insert(gu32 index, const void *data)
{
    cb;
    ca;
    o->InsertElement(o->data,(void *)data,index);
}

bool gLinkedList::isByPass() const
{
    cb false;
    ca;
    return o->bypass;
}

bool gLinkedList::isEmpty() const
{
    cb true;
    ca;
    return o->IsEmpty(o->data);
}


void gLinkedList::moveDownWards(gu32 element)
{
    cb;
    ca;
    o->MoveElementDown(o->data,element);
}
void gLinkedList::moveUpwards(gu32 element)
{
    cb;
    ca;
    o->MoveElementTop(o->data,element);

}


gLinkedList &gLinkedList::operator <<(const void *data)
{
    append(data);
    return *this;
}

gLinkedList &gLinkedList::operator <<(const gLinkedList &other)
{
    append(other);
    return *this;
}

gLinkedList &gLinkedList::operator =(const gLinkedList &other)
{
    clear();
    if(other.isByPass())
    {
        d=other.h();
        ca;
        o->bypass=false;
        o->refCount--;
        o->owner=this;
    }
    else
    {
        append(other);
    }
    return *this;
}

void gLinkedList::remove(gu32 iElement)
{
    cb;
    ca;
    o->DeleteElement(o->data,iElement);
}

void gLinkedList::remove()
{
    cb;
    ca;
    o->DeleteCurrElement(o->data);
}

void gLinkedList::select(gu32 index) const
{
    cb;
    ca;
    o->SetElement(o->data,index);
}

void gLinkedList::markForRemoval()
{
    cb;
    ca;
    o->MarkCurrentForDeletion(o->data);
}

void gLinkedList::markForRemoval(gu32 index)
{
    cb;
    ca;
    o->MarkForDeletion(o->data,index);
}

void gLinkedList::removeMarked()
{
    cb;
    ca;
    o->DeleteMarked(o->data);
}

void gLinkedList::setByPass(bool set)
{
    cb;
    ca;
    o->bypass=set;
    if(set)
        o->refCount++;
}
void gLinkedList::setFunction(IClean func)
{
    cb;
    ca;
    o->func=func;
}

int gLinkedList::index() const
{
    cb -1;
    ca;

    return o->GetCurrentIndex(o->data);
}
