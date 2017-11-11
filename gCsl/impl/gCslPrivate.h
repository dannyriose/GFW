#ifndef GCSLPRIVATE_H
#define GCSLPRIVATE_H
#include <gCsl/gCsl.h>
#include <gArray/gListBase.h>
#define STATE_END 15
#define TOKEN_TYPE_GCSLASSIGN 138
#define TOKEN_TYPE_GCSLASSIGNTO 139
#define CSL_E case TOKEN_TYPE_ERROR: writeError("Error Invalid Token",t);state=STATE_END;break; \
              case TOKEN_TYPE_ENDOF_FILE: writeError("Error Reached end of file");state=STATE_END;break
//Private structure
using namespace gfw;
enum GCSL_LEVEL{
    GCSL_LEVEL_UNASIGNED=0x0fffffff,
    GCSL_LEVEL_IN,
    GCSL_LEVEL_OUT
};

struct gCslLevel:public gListElement{
    gs32 level;
    gs32 iId;
    gCslNode *node;

    gCslLevel():level(0),iId(-1),node(0){

    }
};

struct gCslPrivate:public gSharedObject{
    //! Parse tree
    gTree tree;
    //! Lexer
    gLexer lexer;
    //! This is the error list. We use linked lists because can be expanded dynamically far easier than arrays
    gList<gCslError> errors;
    //! Definition list
    gList<gCslDefinition> definitions;
    //! Open block token
    gs32 openBlock;
    //! Close block token
    gs32 closeBlock;
    //! End of statement
    gs32 endofStatement;
    //! walknode
    gCslNode *walkNode;
    //! Current depth level
    gs32 level;
    //! Last level
    gs32 lastLevel;
    //! Reference count
    gs32 refCount;
    //! Is object shared
    bool bShared;
    //! cancel parsing
    bool bDone;
    //! The token

    //! The list of levels
    gListBase<gCslLevel> levels;
    gCslPrivate():openBlock(TOKEN_TYPE_KEYOPEN),
        closeBlock(TOKEN_TYPE_KEYCLOSE),
        endofStatement(TOKEN_TYPE_DOTCOMA),
        walkNode(0),
        level(1),
        lastLevel(0),
        refCount(1),
        bShared(false),
        bDone(false){

    }
    gCslPrivate(const gCslPrivate &other):
    tree(other.tree),
    lexer(other.lexer),
    errors(other.errors),
    definitions(other.definitions),
    openBlock(other.openBlock),
    closeBlock(other.closeBlock),
    endofStatement(other.endofStatement),
    walkNode(other.walkNode),
    level(other.level),
    lastLevel(other.lastLevel),
    refCount(1),
    bShared(false),
    bDone(false),
    levels(other.levels){

    }

    void writeError(const gString &errmsg,const gToken &t=gToken()){
        gCslError *err=new gCslError();
        err->col=lexer.column();
        err->row=lexer.line();
        err->msg=errmsg;
        err->msg+=" Column:";
        err->msg+=gString::number(err->col);
        err->msg+=gString(" Line:") + gString::number(err->row);
        if(!t.tokenString.isEmpty()){
            err->msg+=gString(" ") + t.tokenString;
        }
        errors.append(err);
    }

    void getToken(){
        gToken &t=lexer.tokenAsRW();
        lexer.getTokenNV();

        switch(t.tokenID1){
        case TOKEN_TYPE_EQUAL:
            lexer.getToken();
            if(t.tokenID1==TOKEN_TYPE_GREATTHAN){
                t.tokenID1=TOKEN_TYPE_GCSLASSIGNTO;
                t.tokenString="=>";
            }else{
                writeError("Expected > after = but got",t);
                lexer.stepBack();
            }
            break;
        case TOKEN_TYPE_LESSTHAN:
            lexer.getToken();
            if(t.tokenID1==TOKEN_TYPE_EQUAL){
                t.tokenID1=TOKEN_TYPE_GCSLASSIGN;
                t.tokenString="<=";
            }else{
                writeError("Expected = after < but got",t);
                lexer.stepBack();
            }
            break;
        }


    }

    gCslNode *addNode(const gString &ident,gCslNode *parent,gs32 id=-1){
        gCslNode *com=new gCslNode(ident,parent,id);
        tree.addNode(com,gCslPrivate::nodeClear);
        return com;
    }
    gCslNode *addNodeWithDef(const gString &ident,gCslNode *parent=0){
        gCslDefinition *def=getDefinition(ident);
        gs32 id=-1;
        if(def){
            id=def->id;
        }
        return addNode(ident,parent,id);
    }
    gCslNode *addParamWithDef(gCslNode *node,const gString &ident,GCSL_NODE_TYPE type){
        gCslDefinition *def=getDefinition(ident);
        gs32 id=-1;
        if(def){
            id=def->id;
        }
        return node->addParam(ident,id,type);
    }

    void setMemberWithDef(gCslNode *node,const gString &ident,GCSL_NODE_TYPE type){
        gCslDefinition *def=getDefinition(ident);
        gs32 id=-1;
        if(def){
            id=def->id;
        }
        node->setMember(ident,id,type);
    }
    void setPropertyWithDef(gCslNode *node,const gString &ident,GCSL_NODE_TYPE type){
        gCslDefinition *def=getDefinition(ident);
        gs32 id=-1;
        if(def){
            id=def->id;
        }
        node->setProperty(ident,id,type);
    }
    void setAssignWithDef(gCslNode *node,const gString &ident,GCSL_NODE_TYPE type){
        gCslDefinition *def=getDefinition(ident);
        gs32 id=-1;
        if(def){
            id=def->id;
        }
        node->setAssign(ident,id,type);
    }
    void setAssignToWithDef(gCslNode *node,const gString &ident,GCSL_NODE_TYPE type){
        gCslDefinition *def=getDefinition(ident);
        gs32 id=-1;
        if(def){
            id=def->id;
        }
        node->setAssignTo(ident,id,type);
    }
    gCslNode *createNode(const gString &ident,gCslNode *parent=0){
        gCslDefinition *def=getDefinition(ident);
        gs32 id=-1;
        if(def){
            id=def->id;
        }
        return new gCslNode(ident,parent,id);
    }

    gCslNode *getNode(){
        return (gCslNode *)tree.nodeData();
    }
    gCslNode *getChildNode(){
        return (gCslNode *)tree.childData();
    }

    gCslDefinition *getDefinition(const gString &id){
        gCslDefinition *def;
        for(gu32 i=0;i<definitions.size();i++){
            def=definitions.pValue(i);
            if(def->ident == id)return def;
        }
        return 0;
    }
    void parseMain(bool bSingleLine){
        bDone=false;
        const gToken &t=lexer.token();
        gCslNode *main=new gCslNode("Script",0,-1);
        tree.create(main,gCslPrivate::nodeClear);
        while(!bDone){
            lexer.getTokenNV();
            switch(t.tokenID1){
            case TOKEN_TYPE_IDENT:parseCommand(main,bSingleLine);break;
            case TOKEN_TYPE_ERROR:writeError("Invalid token ",t);break;
            case TOKEN_TYPE_ENDOF_FILE:bDone=true;break;
            case TOKEN_TYPE_CAT:parseComment();break;
            default:
                writeError("Error: Expecting command string but got ",t);
            }
        }
    }

    gs32 parseCodeBlock(gCslNode *parent){

        bool bDoneCode=false;
        const gToken &t=lexer.token();
        gs32 retval=-1;
        while(!bDone && !bDoneCode){
            getToken();
            switch(t.tokenID1){
            case TOKEN_TYPE_IDENT:
                parseCommand(parent);
                break;
            case TOKEN_TYPE_ERROR:
                break;
            case TOKEN_TYPE_ENDOF_FILE:
                    bDone=true;
                break;
            case TOKEN_TYPE_CAT:
                parseComment();
                break;
            default:
                if(t.tokenID1 == closeBlock){
                    bDoneCode=true;
                    retval=closeBlock;
                }
                break;
                //lexer.stepBack();
            }
        }
        return retval;
    }

    gString parseString(){
        gu32 size=0;
        gString s;
        char c=lexer.currentChar();
        const char *buffer=lexer.scriptAtPos(lexer.position());
        while(c!='"' && c > 0){
            lexer.nextChar();
            c=lexer.currentChar();
            size++;
        }
        lexer.nextChar();//Skip last "
        s.setString(buffer,size);
        s.setShared(true);
        return gString(s);
    }

    void parseComment(){
        char c=lexer.currentChar();
        while(c!='\n' && c > 0){
            lexer.nextChar();
            c=lexer.currentChar();
        }
    }

    gs32 parseParameters(gCslNode *node){
        const gToken &t=lexer.token();
        gCslNode *p;
        int state=0;
        gs32 retval=-1;
        while(state!=STATE_END){
            if(state!=0)
                getToken();
            switch(state){
            case 0://first parameter
            case 2:
                switch(t.tokenID1){
                case TOKEN_TYPE_IDENT:p=addParamWithDef(node,t.tokenString,GCSL_NODE_IDENT);break;
                case TOKEN_TYPE_NUMFLOAT:p=addParamWithDef(node,t.tokenString,GCSL_NODE_FLOAT);break;
                case TOKEN_TYPE_NUMINT:p=addParamWithDef(node,t.tokenString,GCSL_NODE_INTEGER);break;
                case TOKEN_TYPE_COMILLADBL:p=addParamWithDef(node,parseString(),GCSL_NODE_STRING);break;
                CSL_E;
                default:
                   writeError("Expected identifier, number or string but got",t);
                    state=STATE_END;
                    break;
                }
                if(state!=STATE_END)state=1;
                break;
            case 1:
                //check coma separator
                switch(t.tokenID1){
                case TOKEN_TYPE_COMA:state=2;break;
                case TOKEN_TYPE_DOT:state=3;break;
                case TOKEN_TYPE_TWODOTS:state=4;break;
                case TOKEN_TYPE_PAROPEN:state=5;break;//sub parameter
                case TOKEN_TYPE_PARCLOSE:
                case TOKEN_TYPE_GCSLASSIGN:
                case TOKEN_TYPE_GCSLASSIGNTO:
                case TOKEN_TYPE_DOTCOMA:
                case TOKEN_TYPE_KEYOPEN:
                    retval=t.tokenID1;
                    state=STATE_END;
                    break;
                CSL_E;
                default:
                    writeError("Expected , parameter separator but got",t);
                    state=STATE_END;

                }
                break;
            case 3://dot or member
                switch(t.tokenID1){
                case TOKEN_TYPE_IDENT:
                case TOKEN_TYPE_NUMINT:
                    if(p->type!=GCSL_NODE_IDENT){
                        writeError("Cannot add a member to a non ident parameter type");
                        state=STATE_END;
                        break;
                    }
                    if(p->member){
                        writeError("ERROR: Parameter already has a member");
                        state=STATE_END;
                        break;
                    }
                    if(t.tokenID1==TOKEN_TYPE_IDENT){
                        setMemberWithDef(p,t.tokenString,GCSL_NODE_IDENT);
                        p=p->member;
                    }
                    else
                        p->setMember(t.tokenString,-1,GCSL_NODE_INTEGER);

                    state=1;
                    break;
                CSL_E;
                default:
                    writeError(gString("Expected ident on parameter ") + p->ident + " member but got ",t);
                    state=STATE_END;
                }
                break;
            case 4://value
                if(p->type!=GCSL_NODE_IDENT){
                    writeError("Cannot add a value to a non ident parameter type");
                    state=STATE_END;
                    break;
                }
                if(p->property){
                    writeError("ERROR: Parameter already has a Property");
                    state=STATE_END;
                    break;
                }
                switch(t.tokenID1){
                case TOKEN_TYPE_IDENT:setPropertyWithDef(p,t.tokenString,GCSL_NODE_IDENT);break;
                case TOKEN_TYPE_COMILLADBL:setPropertyWithDef(p,parseString(),GCSL_NODE_STRING);break;
                case TOKEN_TYPE_NUMINT:p->setProperty(t.tokenString,-1,GCSL_NODE_INTEGER);break;
                case TOKEN_TYPE_NUMFLOAT:p->setProperty(t.tokenString,-1,GCSL_NODE_FLOAT);break;
                CSL_E;
                default:
                    writeError("Expected value ident,string or numeric value but got",t);
                    state=STATE_END;
                }
                if(state!=STATE_END){
                    state=1;
                    //p=p->value;
                }
                break;
            case 5:
                if(parseParameters(p)!=TOKEN_TYPE_PARCLOSE){
                    writeError(gString("Expected ) on parameter ") + p->ident);
                    state=STATE_END;
                }
                if(state!=STATE_END)state=1;
                break;
            }
        }
        return retval;
    }

    void parseCommand(gCslNode *parent=0, bool bSingleLine = false){
        const gToken &t=lexer.token();
        gCslNode *node;
        int state=0;
        gs32 paramret=-1;
        while(state!=STATE_END){
            if(state!=0)
                getToken();
            switch(state){
            // State 0 parse the command ident
            case 0:
            case 1:
                //Here should not be any error but just in case
                switch(t.tokenID1){
                //! This should always happen
                case TOKEN_TYPE_IDENT:
                    //Add the node
                    node=addNodeWithDef(t.tokenString,parent);
                    walkNode=node;
                    //goto to state 1
                    state=2;
                    break;
                CSL_E;
                }
                break;
            case 2:
                //Checking parameter parsing
                switch(t.tokenID1){
                case TOKEN_TYPE_IDENT:
                case TOKEN_TYPE_COMILLADBL:
                case TOKEN_TYPE_NUMINT:
                case TOKEN_TYPE_NUMFLOAT:
                    //Parse parameter
                    paramret=parseParameters(walkNode);
                    break;
                case TOKEN_TYPE_DOT:state=3;break;
                case TOKEN_TYPE_TWODOTS:state=4;break;
                case TOKEN_TYPE_GCSLASSIGN:state=5;break;
                case TOKEN_TYPE_GCSLASSIGNTO:state=6;break;
                case TOKEN_TYPE_PAROPEN:
                    getToken();
                    paramret=parseParameters(walkNode);
                    if(paramret!=TOKEN_TYPE_PARCLOSE){
                        writeError("Error expected ) but got ",t);
                        state=STATE_END;
                    }
                    paramret=-1;
                    break;
                case TOKEN_TYPE_DOTCOMA:state=STATE_END;break;
                case TOKEN_TYPE_KEYOPEN:
                    node->bHasChild=true;
                    tree.gotoChild();
                    paramret=parseCodeBlock(node);
                    if(paramret!=TOKEN_TYPE_KEYCLOSE){
                        writeError("Expected } but got ");
                    }
                    tree.gotoFather();
                    state=STATE_END;
                    break;
                CSL_E;
                default:
                    writeError("Expected end of sentence ; or { but got",t);
                    state=STATE_END;
                }

                if(state==2){
                    switch(paramret){
                    case TOKEN_TYPE_DOTCOMA:state=STATE_END;break;
                    case TOKEN_TYPE_KEYOPEN:
                        node->bHasChild=true;
                        tree.gotoChild();
                        paramret=parseCodeBlock(node);
                        if(paramret!=TOKEN_TYPE_KEYCLOSE){
                            writeError("Expected } but got ");
                        }
                        tree.gotoFather();
                        state=STATE_END;
                        break;
                    case TOKEN_TYPE_GCSLASSIGN:
                        state=5;
                        break;
                    case TOKEN_TYPE_GCSLASSIGNTO:
                        state=6;
                        break;
                    case TOKEN_TYPE_PARCLOSE:
                        writeError("Unexpected )");
                        state=STATE_END;
                        break;
                    }
                    //added single line to allow parsing single line commands without the dot coma separator
                    if(bSingleLine){
                        //end of line finishes in state = 2
                        state = STATE_END;
                    }
                }
                break;
            case 3:
                if(walkNode->member){
                    writeError("ERROR: Node already has a member");
                    state=STATE_END;
                    break;
                }
                switch(t.tokenID1){
                case TOKEN_TYPE_IDENT:setMemberWithDef(walkNode,t.tokenString,GCSL_NODE_IDENT);break;
                case TOKEN_TYPE_NUMINT:walkNode->setMember(t.tokenString,-1,GCSL_NODE_INTEGER);break;
                 CSL_E;
                default:
                    writeError("Error expecting, ident or integer on node member");
                    state=STATE_END;
                }
                if(state!=STATE_END){
                    state=2;
                    walkNode=walkNode->member;
                }
                break;
            case 4:
                if(walkNode->property){
                    writeError("ERROR: Node already has a property");
                    state=STATE_END;
                    break;
                }
                switch(t.tokenID1){
                case TOKEN_TYPE_IDENT:setPropertyWithDef(walkNode,t.tokenString,GCSL_NODE_IDENT);break;
                case TOKEN_TYPE_COMILLADBL:setPropertyWithDef(walkNode,parseString(),GCSL_NODE_STRING);break;
                case TOKEN_TYPE_NUMINT:walkNode->setProperty(t.tokenString,-1,GCSL_NODE_INTEGER);break;
                case TOKEN_TYPE_NUMFLOAT:walkNode->setProperty(t.tokenString,-1,GCSL_NODE_FLOAT);break;
                CSL_E;
                default:
                    writeError("Error expecting, integer, string or ident o node value");
                    state=STATE_END;
                }
                if(state!=STATE_END){
                    state=2;
                    //walkNode=walkNode->value;
                }
                break;
            case 5:
                if(walkNode->assign){
                    writeError("ERROR: Node already has an assign");
                    state=STATE_END;break;
                }
                switch(t.tokenID1){
                case TOKEN_TYPE_IDENT:setAssignWithDef(walkNode,t.tokenString,GCSL_NODE_IDENT);break;
                case TOKEN_TYPE_COMILLADBL:setAssignWithDef(walkNode,parseString(),GCSL_NODE_STRING);break;
                case TOKEN_TYPE_NUMINT:walkNode->setAssign(t.tokenString,-1,GCSL_NODE_INTEGER);break;
                case TOKEN_TYPE_NUMFLOAT:walkNode->setAssign(t.tokenString,-1,GCSL_NODE_FLOAT);break;
                CSL_E;
                default:
                    writeError("Error expecting, integer, string or ident o node value");
                    state=STATE_END;
                }
                if(state!=STATE_END){
                    state=2;
                    walkNode=walkNode->assign;
                }
                break;
            case 6:
                if(walkNode->assignto){
                    writeError("ERROR: Node already has an assign to");
                    state=STATE_END;break;
                }
                switch(t.tokenID1){
                case TOKEN_TYPE_IDENT:setAssignToWithDef(walkNode,t.tokenString,GCSL_NODE_IDENT);break;
                case TOKEN_TYPE_COMILLADBL:setAssignToWithDef(walkNode,parseString(),GCSL_NODE_STRING);break;
                case TOKEN_TYPE_NUMINT:walkNode->setAssignTo(t.tokenString,-1,GCSL_NODE_INTEGER);break;
                case TOKEN_TYPE_NUMFLOAT:walkNode->setAssignTo(t.tokenString,-1,GCSL_NODE_FLOAT);break;
                CSL_E;
                default:
                    writeError("Error expecting, integer, string or ident o node value");
                    state=STATE_END;
                }
                if(state!=STATE_END){
                    state=2;
                    walkNode=walkNode->assignto;
                }
                break;
            }
        }

    }
    static void nodeClear(void *d){
        gCslNode *c=(gCslNode *)d;
        if(c)delete c;
    }
    virtual void copy(const gSharedObject *other){
        if(!other)return;
        const gCslPrivate *p=static_cast<const gCslPrivate *>(other);
       bDone=p->bDone;
       definitions.copyFromOther(p->definitions);
       errors.copyFromOther(p->errors);
       lastLevel=p->lastLevel;
       level=p->level;
       levels.copyFromOther(p->levels);
       lexer=p->lexer;
       tree=p->tree;
    }
    ~gCslPrivate(){

    }
};

G_SHAREDOBJECT_FACTORY(gCslPrivateFactory,gCslPrivate);
#endif // GCSLPRIVATE_H
