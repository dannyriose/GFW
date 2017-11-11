#ifndef GCSL_H
#define GCSL_H
/***************************G-Command Scripting Language**************/


#include <gfw_global.h>
// Strings
#include <gString/gString.h>
// Lexical Analysis
#include <gLexer/gLexer.h>
#include <gTree/gTree.h>

namespace gfw{
//Going from basic structures to the main tamale
    enum GCSL_NODE_TYPE{
        GCSL_NODE_STRING,
        GCSL_NODE_INTEGER,
        GCSL_NODE_FLOAT,
        GCSL_NODE_IDENT,
        GCSL_NODE_UNSET,
        GCSL_NODE_SIZE
    };


    struct gCslParam{
        //! String identifcator of parameter
        gString ident;
        //! Type of parameter: string, number, floating point number
        GCSL_NODE_TYPE type;
        //! Parameter member if any
        gCslParam *member;
        //! Constructor
        gCslParam():member(0){

        }
        gCslParam(const gString &_ident, GCSL_NODE_TYPE _type):
            ident(_ident),
            type(_type),
            member(0){

        }

        //! Destructor
        ~gCslParam(){
            if(member)
                delete member;
        }
        void setMember(const gString &_ident,GCSL_NODE_TYPE _type){
            member=new gCslParam(_ident,_type);

        }
    };
    //!structure to store command or instruction definitions
    struct gCslDefinition{
        // String to find on script
        gString ident;
        // Unique id indentifier
        gs32 id;
        // The type..is it nested
        gs32 type;
        // A list members that accepts
        gCslDefinition():id(-1),type(GCSL_NODE_UNSET){

        }
        gCslDefinition(const gString &_ident,gs32 _id,gs32 _type):
            ident(_ident),id(_id),type(_type){

        }

    };

    //! Tree node object
    struct SHARED_GFW gCslNode{
        //! Ident of command
        gString ident;
        //! The numeric id for this command if any
        gs32 id;
        //! Type of node
        gs32 type;
        //! The list of parameters of this command
        gList<gCslNode> params;
        //! The member of this command if any
        gCslNode *member;
        //! The value of this node if any
        gCslNode *property;
        //! Assign to or =>
        gCslNode *assignto;
        //! Assign or <=
        gCslNode *assign;
        //! The Parent of a this node
        gCslNode *parent;
        //! Has code block
        bool bHasChild;
        //! The constructor
        gCslNode();
        //! Constructor 2
        gCslNode(const gString &_ident,gCslNode *_parent, gs32 _id=-1,gs32 _type=GCSL_NODE_UNSET);
        //! The destructor
        virtual ~gCslNode();
        //! Searches for a parameter
        bool hasParam(const gString &_ident) const;
        // Parameter access functions
        gCslNode *addParam(const gString &_ident,gs32 _id,GCSL_NODE_TYPE  type);
        //! Gets the current parameter on list and moves to next one
        gCslNode *getParam(gu32 index);
        //! Searches for a param via string
        gCslNode *getParam(const gString & _ident);
        //! Get total count of parameters
        gu32 getParamCount() const;
        //! Sets the member
        void setMember(const gString &_ident,gs32 _id,GCSL_NODE_TYPE _type);
        //! Sets the value
        void setProperty(const gString &_ident,gs32 _id,GCSL_NODE_TYPE _type);
        //! Sets the assign node
        void setAssign(const gString &_ident,gs32 _id,GCSL_NODE_TYPE _type);
        //! Sets the assign-to node
        void setAssignTo(const gString &_ident,gs32 _id,GCSL_NODE_TYPE _type);
    };

    struct gCslError{
        gString msg;
        gu32 col;
        gu32 row;
        gCslError():col(0),row(0){

        }
    };
    //! Console Scripting language is a simple structured language free of context as xml.
    class SHARED_GFW gCsl:public gBaseShared{
    public:
       //! Constructor
       gCsl();
       //! Copy constructor
       gCsl(const gCsl &other);
       //! Destructor
       ~gCsl();

       //! Assign operator
       gCsl &operator = (const gCsl &other);
       //! Opens a file for parsing
       bool openScript(const gString &fileName);
       //! Sets a string buffer as script
       bool setScript(const gString &script);
       //! Gets whether script had errors during parsing
       bool hasErrors() const;
       //! Gets the count of errors found on script
       gu32 getErrorCount() const;
       //! Gets error
       gCslError *getError(gu32 index) const;
       // Parse tree management
       //! Gets the parse tree pointer.
       gTree *getParseTree();
       //! Parses the tree
       void parse(bool bSingleLine = false);
       //! Gets the first from the command line
       gCslNode *getFirstNode();
       // Access to script
       //! Starts "walking" through the script returning the each command statement it finds.
       /** Use this function in a loop like this:
        * while(gcsl.walk()){//do stuff} */
       gCslNode *walk();
       //! Retrieves the ident of the current item selected on walk function
       const gString &nodeName() const;
       //! Retries the id of the current item selected on walk function
       gs32 nodeId() const;
       //! Retrieves the command
       gCslNode *walkNode() const;
       // Tree depth checking
       void checkLevelDepth();
       //! Adds a new depth level for checking
       void addLevel(const gString &slevel);
       //! Sets a level on the current selected command on function walk
       void setLevel(const gString &slevel);
       //! Checks whether current node is in desired leveel
       bool inLevel(const gString &slevel) const;
       //! checks whether current node is out desired level
       bool outLevel(const gString &slevel) const;
       //! Adds a definition
       gCslDefinition *addDefinition(const gString &ident, gs32 id,gs32 type);
       //! Finds a definition
       gCslDefinition *getDefinition(const gString &ident) const;
       //! Gets a definition by list index
       gCslDefinition *getDefinition(gu32 index) const;
       //! total count of definitions
       gu32 getDefinitionCount() const;
       //! Sets the character for end of statement
       void setEndOfStatementToken(gs32 set=TOKEN_TYPE_DOTCOMA);
       void setStartOfBlockCodeToken(gs32 set=TOKEN_TYPE_KEYOPEN);
       void setEndOfBlockCodeToken(gs32 set=TOKEN_TYPE_KEYCLOSE);


    };

}


#endif // GCSL_H
