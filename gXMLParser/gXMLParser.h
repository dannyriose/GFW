/***************************************gXMLParser*************************************************/
/*     FILE:gxmlparser.h
       DESCRIPTION: Implements a light weight xml parser
 */
#ifndef TAGPARSER_H
#define TAGPARSER_H
#include <gfw_global.h>
#include <gString/gStringList.h>
//Inherits all CToken functions
//Parse node elements
#ifdef U_QT
#include <QtCore>
#endif


#define GET_KWORD(word,id,type) if(strcmp(pToken->TokenString,word)==0) \
                                                        {pToken->TokenID2=id;return type;}
#define LEVEL_UNASIGNED 0x7FFFFFFF
#define LEVEL_OUT 0x0FFFFFFF
namespace gfw
{
    enum NODE_TYPES
    {
        TAG_UNASIGNED,
        TAG_NODE_NESTED, //May have child
        TAG_NODE_NESTED_IGNORE,//All content is ignored even if it has tags
        TAG_NODE_SINGLE, //Does not have child
        TAG_NODE_DATA,  //It only contains data
        TAG_NODE_MAIN
    };


    struct SHARED_GFW gXMLAttribute
    {
        gString attName;
        gString szValue;
        int iCol;
        int iRow;
        //Value conversion
        int toInt();
        char *toString();
        float toFloat();
    };

    struct SHARED_GFW gNodeTag
    {
        int iTagID;//Id del nodo
        gString TagName;//Cadena de caracteres del nodo
        gu32 dwType;//Type of node
        gLinkedList Attributes;
        bool bHasClosingTag;
        //Added cols and rows
        int iCol; //Column where tag was found
        int iRow; //Row where tag was found
        gNodeTag *parent;//Parent in the tree
        gNodeTag();//Constructor
    //**************************************/
        void SetFirstAttribute();
        gXMLAttribute *GetAttribute() const;
        const gString &AttributeValue(const char *szAttribute) const;//Returns the value if attribute exists
        gStringList AttributeValues(const gString &attr) const;//returns all attribute values if they exist
        void SetNextAttribute();
        void SetLastAttribute();
        void Create(int iTag,const gString &sName,gu32 dwType,int col=-1,int row=-1, gNodeTag *par=NULL);
        void AddAttribute(const gString &name,const gString &sValue,int col=-1,int row=-1);
        bool HasAttributes() const;
        gu32 GetAttributeCount() const;

        void copyFrom(gNodeTag *src);
        gNodeTag *clone();
        static void freeClone(gNodeTag *t);
    };

    struct SHARED_GFW gNodeTagDefinition
    {
        int iTag;
        gString szIdent;
        gu32 dwType;

        void Create(int iTag,const char *sName,gu32 dwType);
    };

    struct SHARED_GFW gXMLLevel
    {
        int iLevel;
        int iLevelID;
        gString sLevelID;
        gNodeTag *tagL;
        void setLevel(int iLevel, int ID,gNodeTag *lev);
        void setLevel(int iLevel, const char *sLevel,gNodeTag *Lev);

        gXMLLevel();
    };

    struct SHARED_GFW gXMLerror
    {
        gString description;
        gu32 dwCol;
        gu32 dwRow;

    };
    //! gXMLParser is a light weight versatile XML Parser built from scratch.
    /** This simple implementation is capable of Parsing XML as well as HTML.*/
    class SHARED_GFW gXMLParser:public gBaseShared
            #ifdef U_QT
            ,public QObject
            #endif
    {
    #ifdef U_QT
        Q_OBJECT
    #endif
    public:
        gXMLParser();
        gXMLParser(const gXMLParser &other);
    #ifdef U_QT
        gXMLParser(QObject *parent);
    #endif
        ~gXMLParser();
        void create();
        void errorBegin();
        gXMLerror *getError();
        void copyErrors(gList<gXMLerror> &target);
        void copyErrors(gLinkedList &target);

        bool OpenFile(const gString &File);
        void SetTextBuffer(const gString &Buffer);
        void AddTagDefinition(int iIdent,const char *szName,gu32 dwType);
        void PrintStdErr();


        void Parse(bool bAttrLC=true);//Parse data build the parser tree


        //Checks whether all tags are balanced
        bool CheckBalance();

        //The following functions are a set of utilities to access the parse three.
        void GotoMainNode();//Go to father node
        void GotoFatherNode();//
        void GotoChildNode();//
        gNodeTag *GetNodeTag() const;//Gets the current node tag
        gNodeTag *GetChildNodeTag() const;
        gNodeTag *GetFatherNodeTag() const;
        void GetNextChild();////
        void SetFirstChild();//
        void SetLastChild();//
        bool bHasChildNodes() const;//
        gString NodeTagName() const;//
        gu32 NodeTagType() const;//
        int NodeTagID() const;//

        //Get Father data
        gString FatherNodeTagName() const;
        gu32 FatherNodeTagType() const;
        int FatherNodeTagID() const;

        //Get Child data
        gString ChildNodeTagName() const;
        gu32 ChildNodeTagType() const;
        int ChildNodeTagID() const;

        //
        gString GetText() const;
        gString GetChildText();


        //Automatic definitions for some script types
        //! Setup the object to allow HTML definitions which are not allowed in XML
        void SetDefinitionsForHTML();
        //! Setup the basic definitions for XML
        void SetDefinitionsForXML();

        //! Main access function, or script walk function.
        /** It access the parse tree generating levels between top nodes and child nodes. It does not use recursion
         * which makes it very handy. Use this function on a loop like this.
         * while(xmlparser.scriptWalk()){
         *  //do your stuff here
         * }
         * \return current node on script walk.*/
        gNodeTag *scriptWalk();//Start the scripting walk //Returns NULL is all done
        gNodeTag *readTag() const;//Get the tag read by scriptWalk
        const gString &readTagName() const;
        gu32 readTagType() const;
        int readTagID() const;
        const gString &readTagAttribute(const char *sAttribute) const;
        gString readTagText(bool bDeepSearch=true);//Returns the text in the inner tag
        bool readTagClosingTag() const;
        int walkLevel() const;//Returns the current level in the tree

        //Find a specific tag by a parameter
        //Starting by a the currrent value and node
        gNodeTag *FindByType(gu32 dwType);
        gNodeTag *FindByName(const char *szName);
        gNodeTag *FindByNAV(const char *szName,const char *szAttribute,const char *Value);
        gNodeTag *FindByID(int iId);

        gString &GetNodeTagData(const char *tagName);//Prints all node data in script format
        //Generates code from the current position to the child node and returns to
        //previous position
        //Converts all childs data and merge them together into a single text
        gString allToText();
        gString childXML() const;//Returns all html embeded in parent

        bool HasErrors() const;//Returns whether the parser has errors

        //Access to Levels
        // Function scriptWalk generate leves which represents a position inside a markup element
        // The following function help to create and set custom levels for easy access.
        void addLevel(const char *sId);
        void addLevel(int iId);
        bool inLevel(const char *sId);
        bool inLevel(int iId);
        bool isoffLevel(const char *sId);
        bool isoffLevel(int iId);
        bool isUnasignedLevel(const char *sId);
        bool isUnasignedLevel(int iId);
        void checkLevels();
        void removeLevel(const char *sId);
        void removeLevel(int iId);
        void setLevelUnasigned(const char *sId);
        void setLevelUnasigned(int iId);
        void setLevelOut(const char *sId);
        void setLevelOut(int iId);
        bool LevelExist(const char *sId);
        bool LevelExist(int id);
        //! Sets and identification to the current level on the script walk process.
        void setLevel(const char *sId);//Sets the current tag to level if exists if not it creates it
        void setLevel(int id);
        void removeLevels();//Remove all levels
        //************************
        int tagCount() const;

        gXMLParser &operator = (const gXMLParser &other);
    #ifdef U_QT
        private slots:
    #endif
        void cancelOp();//Cancels any operation done in parser function


    };
}
#endif // TAGPARSER_H
