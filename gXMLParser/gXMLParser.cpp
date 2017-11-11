#include "impl/gXMLParserPrivate.h"
using namespace gfw;
#define ca gXMLParserPrivate *o=(gXMLParserPrivate *)d
#define co gXMLParserPrivate *io=(gXMLParserPrivate *)other.d
#define acn gassert(d,"gXMLParser Object empty")
gXMLParser::gXMLParser():gBaseShared(new gXMLParserPrivateFactory){
}

gXMLParser::gXMLParser(const gXMLParser &other):gBaseShared(other,new gXMLParserPrivateFactory){
}

#ifdef U_QT
//Qt support
gXMLParser::gXMLParser(QObject *parent)
{
    connect(parent,SIGNAL(cancel()),this,SLOT(cancelOp()));
}

#endif
gXMLParser::~gXMLParser(){

}

//AddTagDefinition

void gXMLParser::AddTagDefinition(int iIdent,const char *szName, gu32 dwType)
{
    acn;
    ca;
    gNodeTagDefinition *tagdef;
    tagdef=new gNodeTagDefinition;
    tagdef->Create(iIdent,szName,dwType);

    o->tagDefinitions.append(tagdef);
}

//OpenFile
bool gXMLParser::OpenFile(const gString &File)
{
    ca;
    if(!o)
    {
        o=new gXMLParserPrivate;
        d=o;
    }

    return o->lexer.openScript(File);

}
//SetTextBuffer
void gXMLParser::SetTextBuffer(const gString &Buffer)
{
    ca;
    if(!o)
    {
        o=new gXMLParserPrivate;
        d=o;
    }
    o->lexer.setScript(Buffer);
}



//Start Parsing
void gXMLParser::Parse(bool bLCAttr)
{
    acn;
    ca;
    //Principal
    gu32 tokenId;
    bool bDone=false;
    gNodeTag *tag;


    tag=o->fnc_CreateNodeTag(0,"Script",TAG_NODE_MAIN);
    o->parseTree.create(tag,gXMLParserPrivate::CleanNodeTag);//Parent node does not have data it just only represents the main data
    o->bAttributeLC=bLCAttr;
    while(!bDone && !o->bCancel)
    {
        o->fnc_ParseMain();
        o->lexer.getToken();
        tokenId=o->lexer.tokenID1();

        switch(tokenId)
        {
            case TOKEN_TYPE_ERROR:
                o->fnc_WriteError("InvalidToken");
                break;
            case TOKEN_TYPE_ENDOF_FILE:
                bDone=true;
                break;

            default:
                o->lexer.stepBack();
        };

    }

}


void gXMLParser::PrintStdErr()
{
    acn;
    ca;

    gXMLerror *err;

    if(o->errors.isEmpty())
    {
        fprintf(stderr,"Success!!\n");
        return;
    }

    o->errors.first();
    while((err=(gXMLerror *)o->errors.data())!=0)
    {
        fprintf(stderr,"%s \n",err->description.ascii());
        o->errors.next();
    }


}


//Parse tree Access functions

void gXMLParser::GotoMainNode()
{
    acn;
    ca;
    o->parseTree.gotoMain();
}


//
void gXMLParser::GotoFatherNode()
{
    acn;
    ca;
    o->parseTree.gotoFather();
}

//
void gXMLParser::GotoChildNode()
{
    acn;
    ca;
    o->parseTree.gotoChild();
}

//
bool gXMLParser::bHasChildNodes() const
{
    acn;
    ca;
    return o->parseTree.hasChilds();
}

void gXMLParser::GetNextChild()
{
    acn;
    ca;
    o->parseTree.nextChild();
}

void gXMLParser::SetFirstChild()
{
    acn;
    ca;
    o->parseTree.selFirstChild();
}

void gXMLParser::SetLastChild()
{
    acn;
    ca;
    o->parseTree.selLastChild();
}

//Get node tag

gNodeTag *gXMLParser::GetNodeTag() const
{
    acn;
    ca;
    return (gNodeTag *)o->parseTree.nodeData();
}
//
gNodeTag *gXMLParser::GetChildNodeTag() const
{
    acn;
    ca;
    if(o->parseTree.hasChilds() && !o->parseTree.childIsNull())
        return (gNodeTag *)o->parseTree.childData();
    else
        return 0;
}

gNodeTag *gXMLParser::GetFatherNodeTag() const
{
    acn;
    ca;
    return (gNodeTag *)o->parseTree.fatherData();
}

//Get Name
//
gString gXMLParser::NodeTagName() const
{
    gNodeTag *tag;

    tag=GetNodeTag();

    if(tag)
        return gString(tag->TagName,true);
    else
        return gString();
}

//Get Type
gu32 gXMLParser::NodeTagType() const
{
    gNodeTag *tag;

    tag=GetNodeTag();

    if(tag)
        return tag->dwType;
    else
        return 0;
}

//Get ID
int gXMLParser::NodeTagID() const
{
    gNodeTag *tag;

    tag=GetNodeTag();

    if(tag)
        return tag->iTagID;
    else
        return -1;
}

//Father data

gString gXMLParser::FatherNodeTagName() const
{
    gNodeTag *tag;

    tag=GetFatherNodeTag();

    if(tag)
        return gString(tag->TagName,true);
    else
        return gString();
}

//Get Type
gu32 gXMLParser::FatherNodeTagType() const
{
    gNodeTag *tag;

    tag=GetFatherNodeTag();

    if(tag)
        return tag->dwType;
    else
        return 0;
}

//Get ID
int gXMLParser::FatherNodeTagID() const
{
    gNodeTag *tag;

    tag=GetFatherNodeTag();

    if(tag)
        return tag->iTagID;
    else
        return -1;
}

//Child Data
gString gXMLParser::ChildNodeTagName() const
{
    gNodeTag *tag;

    tag=GetChildNodeTag();

    if(tag)
        return gString(tag->TagName);
    else
        return gString();
}

//Get Type
gu32 gXMLParser::ChildNodeTagType() const
{
    gNodeTag *tag;

    tag=GetChildNodeTag();

    if(tag)
        return tag->dwType;
    else
        return 0;
}

//Get ID
int gXMLParser::ChildNodeTagID() const
{
    gNodeTag *tag;

    tag=GetChildNodeTag();

    if(tag)
        return tag->iTagID;
    else
        return -1;
}

//Get Text
gString gXMLParser::GetText() const
{

    if(bHasChildNodes())
    {
        return ChildNodeTagName();


    }


    return gString();
}

//Get child Text
gString gXMLParser::GetChildText()
{
    GotoChildNode();

    if(bHasChildNodes())
    {
      return ChildNodeTagName();




    }
    GotoFatherNode();


    return gString();
}

//Set some definitiosn
//This ignores Scripts and CSS
void gXMLParser::SetDefinitionsForHTML()
{

    AddTagDefinition(0,"meta",TAG_NODE_SINGLE);
    AddTagDefinition(1,"link",TAG_NODE_SINGLE);
    AddTagDefinition(2,"img",TAG_NODE_SINGLE);
    AddTagDefinition(3,"br",TAG_NODE_SINGLE);
    AddTagDefinition(4,"script",TAG_NODE_NESTED_IGNORE);
    AddTagDefinition(5,"style",TAG_NODE_NESTED_IGNORE);
    AddTagDefinition(6,"input",TAG_NODE_SINGLE);
    AddTagDefinition(7,"frame",TAG_NODE_SINGLE);
    AddTagDefinition(8,"hr",TAG_NODE_SINGLE);
    AddTagDefinition(9,"base",TAG_NODE_SINGLE);
    AddTagDefinition(10,"basefont",TAG_NODE_SINGLE);
    AddTagDefinition(11,"col",TAG_NODE_SINGLE);
    AddTagDefinition(12,"param",TAG_NODE_SINGLE);
}


//
void gXMLParser::SetDefinitionsForXML()
{
    return;//Nothing really important to define because its structure of strict
}


//Script Walking

gNodeTag *gXMLParser::scriptWalk()
{
    gNodeTag *tag,*dtag;
    acn;
    ca;
    bool bDone=false;
    dtag=NULL;
     while(!bDone)
     {
         //To store, so lets see how this works
         if(o->m_iLevel<=o->m_lastlevel)
           // GetNextChild();
             o->parseTree.nextChild();
         if(o->m_iLevel>o->m_lastlevel)
             //SetFirstChild();
             o->parseTree.selFirstChild();
          o->m_lastlevel=o->m_iLevel;
          tag=GetChildNodeTag();
        if(tag)
        {
             //GotoChildNode();
            o->parseTree.gotoChild();
             if(bHasChildNodes())
             {
                 o->m_iLevel++;

             }
             else
             {
             //   GotoFatherNode();
                 o->parseTree.gotoFather();
             }
             bDone=true;

        }
        else
        {
            o->m_iLevel--;
            if(o->m_iLevel)
            {
                //GotoFatherNode();
                o->parseTree.gotoFather();
            }
            else
            {
               bDone=true;
            }

        }

    }
    if(o->m_iLevel)
    {
        o->m_walkTag=tag;
         return tag;
    }
    else
    {
         o->m_iLevel=1;
         o->m_lastlevel=0;
         o->m_walkTag=NULL;
        // GotoMainNode();
         o->parseTree.gotoMain();
         return NULL;
    }
}

//READ TAG
gNodeTag *gXMLParser::readTag() const
{
    acn;
    ca;
    return o->m_walkTag;
}

//Read tag name
const gString &gXMLParser::readTagName() const
{
    acn;
    ca;
    if(o->m_walkTag)
    {
        return (const gString &)o->m_walkTag->TagName;
    }
    else
    {
        return o->eref;
    }
}

//Read tag type
gu32 gXMLParser::readTagType() const
{
    acn;
    ca;
    if(o->m_walkTag)
        return o->m_walkTag->dwType;
    else
        return 0;
}


//Read tag type
int gXMLParser::readTagID() const
{
    acn;
    ca;
    if(o->m_walkTag)
        return o->m_walkTag->iTagID;
    else
        return -1;
}

//Searches the attribute on the found tag and returns if found
const gString &gXMLParser::readTagAttribute(const char *sAttribute) const
{
    acn;
    ca;
    if(o->m_walkTag)
        return o->m_walkTag->AttributeValue(sAttribute);
    else
        return o->eref;
}

gString gXMLParser::readTagText(bool bDeepSearch)
{
    acn;
    ca;
    gNodeTag *tag;
    bool bChilds;
    gString retval;
    gu32 dwCurrChild;

    if(o->m_walkTag==o->parseTree.childData())
        return gString();

    dwCurrChild=o->parseTree.childIndex();
    o->parseTree.selFirstChild();
    while((tag=(gNodeTag *)o->parseTree.childData())!=NULL)
    {
        if(tag->dwType==TAG_NODE_DATA)
        {
            retval+=tag->TagName;
        }

        else if(tag->dwType==TAG_NODE_NESTED || tag->dwType==TAG_UNASIGNED)
        {
            //There is a high change we can get something from a nested tag
            if(bDeepSearch)
            {
            //GotoChildNode();
                o->parseTree.gotoChild();
                bChilds=bHasChildNodes();
                if(bChilds)
                {
                   o->m_iLevel++;//Increase the level
                   retval=allToText();
                   o->m_iLevel--;
                }
                o->parseTree.gotoFather();

            }
        }

         o->parseTree.nextChild();

    }
    //return old index
    o->parseTree.selChild(dwCurrChild);
    if(retval.isEmpty()){
        return gString();
    }
    retval.setShared(true);
    return gString(retval);

}

bool gXMLParser::readTagClosingTag() const
{
    acn;
    ca;
    if(o->m_walkTag)
    {
        return o->m_walkTag->bHasClosingTag;
    }
    else
    {
        return false;
    }
}

//Finders
gNodeTag *gXMLParser::FindByType(gu32 dwType)
{
    gNodeTag *tag;
    tag=GetChildNodeTag();
    if(tag)
    {
        if(tag->dwType==dwType)
        {
            return tag;
        }
    }
    else
    {
        return NULL;
    }
    GotoChildNode();
    if(bHasChildNodes())
    {
        tag=FindByType(dwType);
    }
    GotoFatherNode();
    return tag;
}

//Find by Name
gNodeTag *gXMLParser::FindByName(const char *szName)
{
    gNodeTag *tag;
    tag=GetChildNodeTag();
    if(tag)
    {
        if(tag->TagName==szName)
        {
            return tag;
        }
    }
    else
    {
        return NULL;
    }
    GotoChildNode();
    if(bHasChildNodes())
    {
        tag=FindByName(szName);
    }
    GotoFatherNode();
    return tag;
}

//Find by Id
gNodeTag *gXMLParser::FindByID(int id)
{
    gNodeTag *tag;
    tag=GetChildNodeTag();
    if(tag)
    {
        if(tag->iTagID==id)
        {
            return tag;
        }
    }
    else
    {
        return NULL;
    }
    GotoChildNode();
    if(bHasChildNodes())
    {
        tag=FindByID(id);
    }
    GotoFatherNode();
    return tag;
}

//by
gNodeTag *gXMLParser::FindByNAV(const char *szName, const char *szAttribute, const char *Value)
{
    gNodeTag *tag;
    tag=GetChildNodeTag();
    if(tag)
    {
        if(tag->TagName==szName)
        {
            if(tag->AttributeValue(szAttribute)==Value)
            {
                return tag;
            }
            return NULL;
        }
    }
    else
    {
        return NULL;
    }
    GotoChildNode();
    if(bHasChildNodes())
    {
        tag=FindByNAV(szName,szAttribute,Value);
    }
    GotoFatherNode();
    return tag;
}
//All to text:
//Print all data text from child into a string and returns it
gString gXMLParser::allToText()
{
    acn;
    ca;
    gNodeTag *tag;
    gString str;
    int oldLevel;
    int oldLastLevel;

    oldLevel=o->m_iLevel;
    oldLastLevel=o->m_lastlevel;
    bool bDone=false;
     while(!bDone && !o->bCancel)
     {
         //To store, so lets see how this works

         if(o->m_iLevel<=o->m_lastlevel)
            o->parseTree.nextChild();
         if(o->m_iLevel>o->m_lastlevel)
            o->parseTree.selFirstChild();
          o->m_lastlevel=o->m_iLevel;
          tag=(gNodeTag *)o->parseTree.childData();
        if(o->m_iLevel==oldLevel && tag==NULL)
        {
             bDone=true;
             o->m_lastlevel=oldLastLevel;
             break;
        }
        if(tag)
        {
            if(tag->dwType==TAG_NODE_DATA)
            {
                str+=tag->TagName;

            }

            o->parseTree.gotoChild();
             if(bHasChildNodes())
             {
                 o->m_iLevel++;

             }
             else
             {
                o->parseTree.gotoFather();
             }
        }
        else
        {
            o->m_iLevel--;
            if(o->m_iLevel)
            {
                o->parseTree.gotoFather();
            }
            else
            {

            }

        }
    }
     str.setShared(true);
     return gString(str);

}
gString gXMLParser::childXML() const{
    acn;
    ca;
    gNodeTag *tag;
    gString str;
    int oldLevel;
    int oldLastLevel;

    oldLevel=o->m_iLevel;
    oldLastLevel=o->m_lastlevel;
    bool bDone=false;
     while(!bDone && !o->bCancel)
     {
         //To store, so lets see how this works

         if(o->m_iLevel<=o->m_lastlevel)
            o->parseTree.nextChild();
         if(o->m_iLevel>o->m_lastlevel)
            o->parseTree.selFirstChild();
          o->m_lastlevel=o->m_iLevel;
          tag=(gNodeTag *)o->parseTree.childData();
        if(o->m_iLevel==oldLevel && tag==NULL)
        {
             bDone=true;
             o->m_lastlevel=oldLastLevel;
             break;
        }
        if(tag)
        {
            if(tag->dwType==TAG_NODE_DATA)
            {
                str+=tag->TagName;

            }
            else if(tag->dwType!=TAG_NODE_DATA && tag->dwType!=TAG_NODE_MAIN){
                str+="<";
                str+=tag->TagName;

                //yeah now print the fucking attributes
                tag->Attributes.first();
                gXMLAttribute *a;
                while((a=(gXMLAttribute *)tag->Attributes.data())!=0){
                    str+=" ";
                    str+=a->attName;
                    str+="=\"";
                    str+=a->szValue;
                    str+="\"";
                    tag->Attributes.next();
                }
                if(tag->dwType==TAG_NODE_SINGLE)
                    str+=" />";
                else
                    str+=">";

            }

            o->parseTree.gotoChild();
             if(bHasChildNodes())
             {
                 o->m_iLevel++;

             }
             else
             {
                o->parseTree.gotoFather();
             }
        }
        else
        {
            o->m_iLevel--;
            if(o->m_iLevel)
            {
                if(o->m_iLevel>=oldLevel){
                    tag=(gNodeTag *)o->parseTree.nodeData();
                    if(tag){
                        //print the closing
                        str+="</";
                        str+=tag->TagName;
                        str+=">";
                    }
                }
                o->parseTree.gotoFather();
            }
            else
            {

            }

        }
    }
     str.setShared(true);
     return gString(str);

}
//INT
int gXMLParser::walkLevel() const
{
    acn;
    ca;
    return o->m_iLevel;
}



void gXMLParser::cancelOp()
{
    acn;
    ca;
    o->bCancel=true;
}


bool gXMLParser::HasErrors() const
{
    acn;
    ca;

    return o->errors.isEmpty() == false;
}

void gXMLParser::addLevel(const char *sId)
{
    acn;
    ca;
    gXMLLevel *l;

    l=new gXMLLevel;

    l->setLevel(o->m_iLevel,sId,o->m_walkTag);
    o->levels.append(l);

}


void gXMLParser::addLevel(int iId)
{
    acn;
    ca;
    gXMLLevel *l;

    l=new gXMLLevel;

    l->setLevel(o->m_iLevel,iId,o->m_walkTag);
    o->levels.append(l);

}


bool gXMLParser::inLevel(const char *sId)
{
    acn;
    ca;
    gXMLLevel *l;

    o->levels.first();
    while((l=(gXMLLevel *)o->levels.data())!=0)
    {
        if(l->sLevelID==sId)
        {
            return o->m_iLevel>=l->iLevel;
        }
        o->levels.next();
    }

    return false;
}

bool gXMLParser::inLevel(int iId)
{
    acn;
    ca;
    gXMLLevel *l;

    o->levels.first();
    while((l=(gXMLLevel *)o->levels.data())!=0)
    {
        if(l->iLevelID==iId)
        {
            return o->m_iLevel>=l->iLevel;
        }
        o->levels.next();
    }

    return false;

}

void gXMLParser::checkLevels()
{
    acn;
    ca;
    gXMLLevel *l;

    if(!o->m_walkTag) return;
    o->levels.first();
    while((l=(gXMLLevel *)o->levels.data())!=0)
    {

        if(l->iLevel>=o->m_iLevel && o->m_walkTag->parent!=l->tagL && l->iLevel!=LEVEL_UNASIGNED)
        {
            l->iLevel=LEVEL_OUT;
        }

        o->levels.next();
    }

}

void gXMLParser::removeLevel(const char *sId)
{
    acn;
    ca;
    gXMLLevel *l;

    o->levels.first();
    while((l=(gXMLLevel *)o->levels.data())!=0)
    {
        if(l->sLevelID==sId)
        {
            o->levels.remove();
            return;
        }
        o->levels.next();
    }

}

void gXMLParser::removeLevel(int id)
{
    acn;
    ca;
    gXMLLevel *l;

    o->levels.first();
    while((l=(gXMLLevel *)o->levels.data())!=0)
    {
        if(l->iLevelID==id)
        {
            o->levels.remove();
            return;
        }
        o->levels.next();
    }

}

void gXMLParser::setLevelUnasigned(const char *sId)
{
    acn;
    ca;
    gXMLLevel *l;

    o->levels.first();
    while((l=(gXMLLevel *)o->levels.data())!=0)
    {
        if(l->sLevelID==sId)
        {
            l->iLevel=LEVEL_UNASIGNED;
            return;
        }
        o->levels.next();
    }

}

void gXMLParser::setLevelUnasigned(int iId)
{
    acn;
    ca;
    gXMLLevel *l;

    o->levels.first();
    while((l=(gXMLLevel *)o->levels.data())!=0)
    {
        if(l->iLevelID==iId)
        {
            l->iLevel=LEVEL_UNASIGNED;
            return;
        }
        o->levels.next();
    }
}


void gXMLParser::setLevelOut(const char *sId)
{
    acn;
    ca;
    gXMLLevel *l;

    o->levels.first();
    while((l=(gXMLLevel *)o->levels.data())!=0)
    {
        if(l->sLevelID==sId)
        {
            l->iLevel=LEVEL_OUT;
            return;
        }
        o->levels.next();
    }


}

void gXMLParser::setLevelOut(int iId)
{
    acn;
    ca;
    gXMLLevel *l;

    o->levels.first();
    while((l=(gXMLLevel *)o->levels.data())!=0)
    {
        if(l->iLevelID==iId)
        {
            l->iLevel=LEVEL_OUT;
            return;
        }
        o->levels.next();
    }

}

bool gXMLParser::isoffLevel(const char *sId)
{
    acn;
    ca;
    gXMLLevel *l;

    o->levels.first();
    while((l=(gXMLLevel *)o->levels.data())!=0)
    {
        if(l->sLevelID==sId)
        {
            return l->iLevel==LEVEL_OUT;
        }
        o->levels.next();
    }

    return false;
}


bool gXMLParser::isoffLevel(int iId)
{
    acn;
    ca;
    gXMLLevel *l;

    o->levels.first();
    while((l=(gXMLLevel *)o->levels.data())!=0)
    {
        if(l->iLevelID==iId)
        {
            return l->iLevel==LEVEL_OUT;
        }
        o->levels.next();
    }

    return false;
}


bool gXMLParser::isUnasignedLevel(const char *sId)
{
    acn;
    ca;
    gXMLLevel *l;

    o->levels.first();
    while((l=(gXMLLevel *)o->levels.data())!=0)
    {
        if(l->sLevelID==sId)
        {
            return l->iLevel==LEVEL_UNASIGNED;
        }
        o->levels.next();
    }

    return false;
}


bool gXMLParser::isUnasignedLevel(int iId)
{
    acn;
    ca;
    gXMLLevel *l;

    o->levels.first();
    while((l=(gXMLLevel *)o->levels.data())!=0)
    {
        if(l->iLevelID==iId)
        {
            return l->iLevel==LEVEL_UNASIGNED;
        }
        o->levels.next();
    }

    return false;
}

bool gXMLParser::LevelExist(const char *sId)
{
    acn;
    ca;
    gXMLLevel *l;

    o->levels.first();
    while((l=(gXMLLevel *)o->levels.data())!=0)
    {
        if(l->sLevelID==sId)
        {
            return true;
        }
        o->levels.next();
    }

    return false;
}

bool gXMLParser::LevelExist(int id)
{
    acn;
    ca;
    gXMLLevel *l;

    o->levels.first();
    while((l=(gXMLLevel *)o->levels.data())!=0)
    {
        if(l->iLevelID==id)
        {
            return true;
        }
        o->levels.next();
    }

    return false;
}

void gXMLParser::setLevel(const char *sId)
{
    acn;
    ca;
    bool bExists=false;
    gXMLLevel *l;
    o->levels.first();
    while((l=(gXMLLevel *)o->levels.data())!=0)
    {
        if(l->sLevelID==sId)
        {
            bExists=true;
            l->iLevel=o->m_iLevel;
            l->tagL=o->m_walkTag;
        }
        o->levels.next();
    }
    if(!bExists)
       addLevel(sId);

}

void gXMLParser::setLevel(int iId)
{
    acn;
    ca;
    bool bExists=false;
    gXMLLevel *l;
    o->levels.first();
    while((l=(gXMLLevel *)o->levels.data())!=0)
    {
        if(l->iLevelID==iId)
        {
            bExists=true;
            l->iLevel=o->m_iLevel;
            l->tagL=o->m_walkTag;
        }
        o->levels.next();
    }
    if(!bExists)
       addLevel(iId);

}

void gXMLParser::removeLevels()
{
    acn;
    ca;
    o->levels.clear();
}

/************************************************/
void gXMLParser::errorBegin()
{
    acn;
    ca;
    o->errors.first();
}

gXMLerror *gXMLParser::getError()
{
    acn;
    ca;
    gXMLerror *e;
    e=(gXMLerror *)o->errors.data();

    o->errors.next();
    return e;

}

/*****************Copying the error list to a list******/
void gXMLParser::copyErrors(gLinkedList &target)
{
   acn;
   ca;
   gXMLerror *e,*ne;
   o->errors.first();
   while((e=(gXMLerror *)o->errors.data())!=0)
   {
       ne=new gXMLerror;

       ne->description=e->description;
       ne->dwCol=e->dwCol;
       ne->dwRow=e->dwRow;
       target.append(ne);
       o->errors.next();
   }
}

void gXMLParser::copyErrors(gList<gXMLerror> &list)
{
    acn;
    ca;
    gu32 i=0;
    gXMLerror *e,*ne;
    if(o->errors.isEmpty())return;
    o->errors.first();
    list.alloc(o->errors.count());
    while((e=(gXMLerror *)o->errors.data())!=0)
    {
        ne=new gXMLerror;

        ne->description=e->description;
        ne->dwCol=e->dwCol;
        ne->dwRow=e->dwRow;
        list.setValue(ne,i);
        o->errors.next();
        i++;
    }
}

int gXMLParser::tagCount() const
{
    acn;
    ca;
    return o->m_tagCount;
}





gXMLParser &gXMLParser::operator =(const gXMLParser &other){
    assign(other);
    return *this;
}
