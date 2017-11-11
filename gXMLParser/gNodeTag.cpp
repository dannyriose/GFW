#include "gXMLParser.h"

using namespace gfw;

SHARED_GFW const gString emptyRef;
//Attribute killer
void CleanAttributeList(void *p)
{
    gXMLAttribute *attr=(gXMLAttribute *)p;

    if(attr) delete attr;
}

gNodeTag::gNodeTag()
{
    Attributes.setFunction(CleanAttributeList);
    bHasClosingTag=false;
}

//Create

void gNodeTagDefinition::Create(int Tag, const char *sName, gu32 dwT)
{
    iTag=Tag;
    szIdent=sName;
    dwType=dwT;
}

//Create
void gNodeTag::Create(int iTag, const gString &sName, gu32 dwT,int col,int row,gNodeTag *par)
{
    iTagID=iTag;
    TagName=sName;
    dwType=dwT;
    iCol=col;
    iRow=row;
    parent=par;
}
//Add Attribute
void gNodeTag::AddAttribute(const gString &sName, const gString &sValue,int col,int row)
{
    gXMLAttribute *attr;

    attr=new gXMLAttribute;

    attr->attName=sName;
    attr->szValue=sValue;
    attr->iCol=col;
    attr->iRow=row;
    Attributes.append(attr);
}

//Done for NodeTag
//SetFirstAttribute
void gNodeTag::SetFirstAttribute()
{
    Attributes.first();
}

//GetAttribute
gXMLAttribute *gNodeTag::GetAttribute() const
{
    return (gXMLAttribute *)Attributes.data();
}


void gNodeTag::SetLastAttribute()
{
    Attributes.last();
}

//SetNextAttribute
void gNodeTag::SetNextAttribute()
{
    Attributes.next();
}

//HasAttributes
bool gNodeTag::HasAttributes() const
{
    return Attributes.isEmpty()==false;
}

gu32 gNodeTag::GetAttributeCount() const
{
    return Attributes.count();
}

//
const gString &gNodeTag::AttributeValue(const char *szAttribute) const
{
    gXMLAttribute *attr;

    Attributes.first();

    while((attr=(gXMLAttribute *)Attributes.data())!=0)
    {
        if(attr->attName==szAttribute)
            return (const gString &)attr->szValue;
        Attributes.next();
    }

    return emptyRef;
}
gStringList gNodeTag::AttributeValues(const gString &attr) const{
    gXMLAttribute *a;
    gu32 i;
    gStringList cop;
    gStringList out;

    cop=attr.split(",");
    out.alloc(cop.size());
    out.fill();//avoid messing around with null references
    Attributes.first();
    while((a=(gXMLAttribute *)Attributes.data())!=0){
        for(i=0;i<cop.size();i++){

            if(cop[i]==a->attName){
                out.setValue(a->szValue,i);
                break;
            }

        }
        Attributes.next();
    }
    out.setShared(true);
    return gStringList(out);
}

void gNodeTag::copyFrom(gNodeTag *src)
{
    gXMLAttribute *a;
    if(!src)
        return;
    iTagID=src->iTagID;
    bHasClosingTag=src->bHasClosingTag;
    dwType=src->dwType;
    TagName=src->TagName;
    Attributes=src->Attributes;
    iCol=src->iCol;
    iRow=src->iRow;
    parent=src->parent;

            //Copy completed
}

gNodeTag *gNodeTag::clone()
{
    gNodeTag *nt;

    nt=new gNodeTag;

    if(nt)
    {
        nt->copyFrom(this);
    }

    return nt;
}

void gNodeTag::freeClone(gNodeTag *t)
{
    if(t)
        delete t;
}
