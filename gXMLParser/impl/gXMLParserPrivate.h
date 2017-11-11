#ifndef GXMLPARSERPRIVATE_H
#define GXMLPARSERPRIVATE_H
#include <gXMLParser/gXMLParser.h>
#include <gLinkedList/gtLinkedList.h>
#include <gTree/gTree.h>
#include <gLexer/gLexer.h>
//List destructors
namespace gfw{

//Clean Tag Node
//data
//private structure
struct gXMLParserPrivate:public gSharedObject
{
    gTree parseTree;//The parse tree
    gLinkedList tagDefinitions;//Tag definitions
    gLinkedList errors;//List of errors
    gLinkedList levels;//List of registered levels
    gLexer lexer;//the lexical analyzer
    const gString eref;
    gu32 fnc_GetReservedWord();
    void fnc_WriteError(const char *);
    void fnc_WriteError(const char *,const gString &str);

    void fnc_ParseMain();
    void fnc_ParseTag();
    void fnc_ParseComment();
    void fnc_ParseCDATA();
    void fnc_ParseAttribute();
    void fnc_IgnoreAndSave(char *ignoreBounds,int size);
    void fnc_JumpToFinalTag();//Jumps directly to the final tag if exists
    gNodeTag *fnc_CreateNodeTag(int iTag,const char *sName,gu32 dwType);
    //Error reporting
    //Tree walking
    int m_iLevel;//Current level in tree walking
    int m_lastlevel;
    int m_tagCount;
    gNodeTag *m_walkTag;
    bool bCancel;
    bool bAttributeLC;//if true all attributes are lower case

    gXMLParserPrivate():m_iLevel(1),m_lastlevel(0),m_tagCount(0),m_walkTag(0),bCancel(0)
    {
        tagDefinitions.setFunction(CleanTagDefinitions);
        errors.setFunction(CleanErrorList);
        levels.setFunction(LevelClean);
    }

    gNodeTag *GetNodeTag() const
    {
        return (gNodeTag *)parseTree.nodeData();
    }
    virtual void copy(const gSharedObject *other){
        GFW_PRIVATE_CAST(o,other,gXMLParserPrivate);
        bAttributeLC=o->bAttributeLC;
        bCancel=o->bCancel;
        errors=o->errors;
        parseTree=o->parseTree;
        tagDefinitions=o->tagDefinitions;
        lexer=o->lexer;
        levels=o->levels;
        m_lastlevel=o->m_lastlevel;
        m_tagCount=o->m_tagCount;
        m_walkTag=0;//cant copy this
    }
    static void CleanNodeTag(void *pData)
    {
        gNodeTag *tag=(gNodeTag *)pData;

        if(tag) delete tag;
    }

    static void CleanErrorList(void *pData)
    {
        gXMLerror *str=(gXMLerror *)pData;
        if(str) delete str;
    }

    static void CleanTagDefinitions(void *pData)
    {
        gNodeTagDefinition *tag=(gNodeTagDefinition *)pData;

        if(tag) delete tag;
    }

    //Level cleaner
    static void LevelClean(void *p)
    {
        gXMLLevel *l=(gXMLLevel *)p;

        if(l)
            delete l;
    }

};
G_SHAREDOBJECT_FACTORY(gXMLParserPrivateFactory,gXMLParserPrivate);
}
#endif // GXMLPARSERPRIVATE_H
