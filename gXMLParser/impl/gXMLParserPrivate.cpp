#include "gXMLParserPrivate.h"
using namespace gfw;

//fnc_GetReservedKeyword

gu32 gXMLParserPrivate::fnc_GetReservedWord()
{

    gNodeTagDefinition *def;

    tagDefinitions.first();
    while((def=(gNodeTagDefinition *)tagDefinitions.data())!=0)
    {
        if(lexer.tokenString()==def->szIdent)
        {
            lexer.setTokenID2(def->iTag);
            return def->dwType;
        }
        tagDefinitions.next();
    }


    return TAG_UNASIGNED;
}

//fnc_WriteError
void gXMLParserPrivate::fnc_WriteError(const char *s)
{
    gXMLerror *val;
    gu32 col,line;
    val=new gXMLerror;

    col=lexer.column();
    line=lexer.line();
    val->description=s;
    val->description+=" Col:";
    val->description+= gString::number(col);
    val->description+=" Line:";
    val->description+= gString::number(line);
    val->description+=" ";
    val->dwCol=col;
    val->dwRow=line;
    val->description+=lexer.tokenString();

    errors.append(val);
}

//fnc_WriteError
void gXMLParserPrivate::fnc_WriteError(const char *s,const gString &msg)
{
    gXMLerror *val;

    gu32 dwCol,dwLine;
    val=new gXMLerror;

    dwCol=lexer.column();
    dwLine=lexer.line();

    val->description=s;
    val->description+=" Col:";
    val->description+= gString::number(dwCol);
    val->description+=" Line:";
    val->description+= gString::number(dwLine);
    val->description+=" ";
    val->description+=msg;
    val->dwCol=dwCol;
    val->dwRow=dwLine;
    errors.append(val);
}

//Parse Main<p>my tomy pat</p>
void gXMLParserPrivate::fnc_ParseMain()
{

    const gToken &token=lexer.getToken();

    switch(token.tokenID1)
    {

    case TOKEN_TYPE_LESSTHAN: //<//Means start of tag
        fnc_ParseTag();
        break;
    case TOKEN_TYPE_ERROR:
        break;
    case TOKEN_TYPE_ENDOF_FILE:
        break;

    default:
        //any character that may be saved and add it to the current
        lexer.stepBack();
        fnc_IgnoreAndSave("<",1);

        break;
    }

}


//Void parsetag
//<A>
void gXMLParserPrivate::fnc_ParseTag()
{
    int state=0;//for the machine state
    gNodeTag *tag=NULL;//The node tag

    gu32 dwType=0;//Got the type
    gString ident;
    int iIdent;

    while(state!=15 && !bCancel)
    {
        const gToken &pToken=lexer.getToken();
        switch(state)
        {
        case 0://We expect three different tokens an ident a / or a ?
            switch(pToken.tokenID1)
            {
            case TOKEN_TYPE_IDENT://Got an ident
                if(!dwType)
                    dwType=fnc_GetReservedWord();
                iIdent=pToken.tokenID2;
                ident+=pToken.tokenString;//Store it on ident
                state=1;//Set the next state
                break;
            case TOKEN_TYPE_DIAGONAL://Expected when it is about an ending tag
                //Set state to the ending tag phase
                state=2;//cool
                break;
            case TOKEN_TYPE_QUESTIONCLOSE://Used on start of XML documents
                //Set the type to SINGLE
                dwType=TAG_NODE_SINGLE;
                //state remains in zero
                break;
            case TOKEN_TYPE_EXCLAMATIONCLOSE:
                //it is a comment
                fnc_ParseComment();
                state=15;
                break;
            case TOKEN_TYPE_ENDOF_FILE:
                state=15;//reached end of script
                fnc_WriteError("Unexpected end of script");
                break;
            case TOKEN_TYPE_ERROR:
                //ignore crap
                break;

            default:
                //Anything else means an error and must be set
                fnc_WriteError("ERROR: Unexpected ");
                //could be any wrong character so we out
                //state=15;

            }
            //end of state 0
            break;
        case 1://expected from the ident
            switch(pToken.tokenID1)
            {
            case TOKEN_TYPE_IDENT:
                //we got another identificator this means we are facing a set of attributes
                //we rewind the token and call to attribute
                lexer.stepBack();
                //we create the node tag
                //set ident to lower
                ident.thisToLowCase();//need this function
                tag=fnc_CreateNodeTag(iIdent,ident.ascii(),dwType);
                //we add the node

                parseTree.addNode(tag,CleanNodeTag);
                //Go to the  newly created tag in the tree
                parseTree.gotoChild();
                fnc_ParseAttribute();

                break;
                //it happens in XML
            case TOKEN_TYPE_TWODOTS://so we only attatch the simbol to the name
                ident+=":";//and return the state to 0
                state=0;
                break;
            case TOKEN_TYPE_DIAGONAL://Facint the end of a single node tag for some strict HTML and XML documents
                //we store the data
                //At this point the tag should be created
                     //Set the type as single
                dwType=TAG_NODE_SINGLE;
                //waiting on state 1 for the closing tag >
                break;
            case TOKEN_TYPE_QUESTIONCLOSE:
                //Ignore
                break;
            case TOKEN_TYPE_GREATTHAN://End of tag
                if(tag)
                {
                    if(dwType==TAG_NODE_SINGLE)
                    {
                        tag->dwType=dwType;
                        tag->bHasClosingTag=true;
                        if(tag->HasAttributes())
                            parseTree.gotoFather();
                    }
                }
                else
                {
                    //if tag has not been created we create it
                    ident.thisToLowCase();
                    tag=fnc_CreateNodeTag(iIdent,ident.ascii(),dwType);
                    parseTree.addNode(tag,CleanNodeTag);

                    if(dwType==TAG_NODE_SINGLE)
                    {
                        tag->bHasClosingTag=true;
                    }
                    else
                    {
                        parseTree.gotoChild();
                    }

                }
                // if the document the content of the inner tag is ment to be ignore we use the jump function
                if(dwType==TAG_NODE_NESTED_IGNORE)
                    fnc_JumpToFinalTag();
                //Set the state to end
                state=15;
                break;
            case TOKEN_TYPE_ENDOF_FILE:
                state=15;//reached end of script
                fnc_WriteError("Unexpected end of script");
                break;
            case TOKEN_TYPE_ERROR:
                //ignore crap
                break;
            }
            break;//End of state 1
        case 2://Facing a ending tag
            //We expect the end of a tag
            tag=GetNodeTag();//get the current tag
            switch(pToken.tokenID1)
            {
            case TOKEN_TYPE_IDENT:
                //We compare the ident found
                ident+=pToken.tokenString;
                ident.thisToLowCase();//to lower case
                if(tag->TagName==ident)
                {
                   tag->bHasClosingTag=true;
                   //Go to father
                   //suppose to be nested?
                   if(tag->dwType==TAG_UNASIGNED)
                       tag->dwType=TAG_NODE_NESTED;
                   parseTree.gotoFather();
                    state=3;//Go to state 3
                }
                else
                {
                 //This means we are facing an error of unbalanced tags or the tag is composed with :
                    //and go to state 3
                    state=3;

                }
                break;
            case TOKEN_TYPE_ENDOF_FILE:
                state=15;//reached end of script
                fnc_WriteError("Unexpected end of script");
                break;
            case TOKEN_TYPE_ERROR:
                //ignore crap
                break;

            }
            break;
        case 3:
            switch(pToken.tokenID1)
            {
            case TOKEN_TYPE_GREATTHAN:
                //Anything went great or not
                if(!tag->bHasClosingTag)
                    fnc_WriteError("WARNING:Unbalanced tag",tag->TagName);
                state=15;
                break;
            case TOKEN_TYPE_TWODOTS:
                ident+=":";
                //go back to state 2
                state=2;//we xpect again an ident on 2
                break;
            case TOKEN_TYPE_ENDOF_FILE:
                state=15;//reached end of script
                fnc_WriteError("Unexpected end of script");
                break;
            case TOKEN_TYPE_ERROR:
                //ignore crap
                break;

            }
        }

    }

}

void gXMLParserPrivate::fnc_ParseAttribute()
{
    //Parsing attributes
    char c;
    int state=0;
    gu32 dwSize;
    char *sBuffer;
    gNodeTag *tag;
    gString ident;
    gString value;
    tag=GetNodeTag();
    while(state!=15 && !bCancel)
    {
        const gToken &pToken=lexer.getToken();
        switch(state)
        {
        case 0://Ident
            switch(pToken.tokenID1)
            {
            case TOKEN_TYPE_IDENT:

                ident+=pToken.tokenString;//Got the ident

                //go to next state
                state=1;
                break;
            case TOKEN_TYPE_GREATTHAN://Facing end of tag
                lexer.stepBack();//Go back
                state=15;
                break;
            case TOKEN_TYPE_QUESTIONCLOSE:
                //Ignore
                break;
            case TOKEN_TYPE_DIAGONAL://Facing end of tag of single node tags of  strict documents
                lexer.stepBack();
                state=15;
                break;
            case TOKEN_TYPE_ENDOF_FILE:
                state=15;//reached end of script
                fnc_WriteError("Unexpected end of script");
                break;
            case TOKEN_TYPE_ERROR:
                //ignore crap
                break;
            }
            break;//End of state 0
        case 1:
            switch(pToken.tokenID1)
            {
            case TOKEN_TYPE_EQUAL:
                //Got the equal go to state 2
                state=2;
                break;
            case TOKEN_TYPE_TWODOTS://For some XML CASES
                ident+=":";
                //go to sate 0
                state=0;
                break;
            case TOKEN_TYPE_ENDOF_FILE:
                state=15;//reached end of script
                fnc_WriteError("Unexpected end of script");
                break;
            case TOKEN_TYPE_ERROR:
                //ignore crap
                break;
            }
            break;
            //end of state 1
        case 2:
            switch(pToken.tokenID1)
            {
            case TOKEN_TYPE_COMILLADBL:
                dwSize=0;

                sBuffer=lexer.scriptAtPos(lexer.position());
                c=lexer.currentChar();
                while(c!='"' && c>0)
                {
                    //value+=GetNextChar(); Instead of concat we find out the buffersize
                    lexer.nextChar();
                    c=lexer.currentChar();
                    dwSize++;
                }
                lexer.nextChar();
                //add the value
                ident.thisToLowCase();
                //value.copyfromMem(sBuffer,dwSize);
                value.setString(sBuffer,dwSize);
                if(bAttributeLC)
                    value.thisToLowCase();

                tag->AddAttribute(ident,value,lexer.column(),lexer.line());
                //reset value
                value.clear();
                //go back to state 0
                state=0;
                break;
            case TOKEN_TYPE_COMILLASIN:
                dwSize=0;
                sBuffer=lexer.scriptAtPos(lexer.position());
                c=lexer.currentChar();
                while(c!=39 && c>0)
                {
                   // value+=GetNextChar();
                    lexer.nextChar();
                    c=lexer.currentChar();
                    dwSize++;
                }
                lexer.nextChar();
                ident.thisToLowCase();
                value.setString(sBuffer,dwSize);
                if(bAttributeLC)
                    value.thisToLowCase();

                tag->AddAttribute(ident,value,lexer.column(),lexer.line());
                value.clear();
                state=0;
                break;
             case TOKEN_TYPE_IDENT:
                //Some documents have a lack of sintax on attributes so we parse they attributes as idents
                value=pToken.tokenString;
                //to lower everything
                ident.thisToLowCase();
                if(bAttributeLC)
                    value.thisToLowCase();
                tag->AddAttribute(ident,value,lexer.column(),lexer.line());
                state=0;
                break;
            case TOKEN_TYPE_CAT:
                //ignore and remain on state 2

                break;
            case TOKEN_TYPE_ENDOF_FILE:
                state=15;//reached end of script
                fnc_WriteError("Unexpected end of script");
                break;
            case TOKEN_TYPE_ERROR:
                //ignore crap
                break;

            }
            //!fixes bug when states remains 2 either by token type cat or token type error
            if(state != 2)
            {
                ident.clear();
            }
            break;

        }


    }


}

//Parse comments
void gXMLParserPrivate::fnc_ParseComment()
{
   // const gToken **tToken=0;
    const gToken &pToken=lexer.token();
    int state=0;

    char c;//look
    while(state!=15 && !bCancel)
    {
        if(state!=1) //Happens when comment data is ignored so no need to getoken
            lexer.getToken();
        switch(state)
        {
        case 0:
            //expecting -
            switch(pToken.tokenID1)
            {
            case TOKEN_TYPE_GUIONNORMAL:
                state=4;//Goto checking for the other guion
                break;
            case TOKEN_TYPE_COROPEN://Dealing with CDATA
                fnc_ParseCDATA();
                state=15;
                break;
            case TOKEN_TYPE_IDENT://dealing with other unknown data such as the start of a HTML document
                //ignore it
                state=3;
                break;
            case TOKEN_TYPE_ENDOF_FILE:
                state=15;//reached end of script
                fnc_WriteError("Unexpected end of script");
                break;
            case TOKEN_TYPE_ERROR:
                //ignore crap
                break;
            }
            break;
        case 1:
            //Ignoring data
            c=lexer.currentChar();
            while(c!='-' && c!=0)//Zero means end of file
            {
                c=lexer.nextChar();
            }
            //Skip previous
            //c=lexer.nextChar();
            if(lexer.nextChar()=='-')
            //Go out
                state=2;
            if(c==-125)
            {
                fnc_WriteError("Unexpected end of file");
                state=15;
            }
            //Stay if not
            break;
        case 2:
            switch(pToken.tokenID1)

            {
            case TOKEN_TYPE_GREATTHAN:
                state=15;
                break;
            case TOKEN_TYPE_ENDOF_FILE:
                state=15;//reached end of script
                fnc_WriteError("Unexpected end of script");
                break;
            case TOKEN_TYPE_ERROR:
                fnc_WriteError("Crap found");
                break;
            default:
                state=1;
                //fnc_WriteError("Expecting > at end of comment",pToken);
            }

            break;
        case 3:
            //Ignoring data
            c=lexer.currentChar();
            while(c!='>' && c!=0)//Zero means end of file
            {
                c=lexer.nextChar();
            }
            lexer.nextChar();
            state=15;
            break;
        case 4:
            switch(pToken.tokenID1)
            {
            case TOKEN_TYPE_GUIONNORMAL:
                state=1;//Comment checking completed
                break;

            case TOKEN_TYPE_ENDOF_FILE:
                state=15;//reached end of script
                fnc_WriteError("Unexpected end of script");
                break;
            case TOKEN_TYPE_ERROR:
                //ignore crap
                break;
            }


        }

    }
}


//PArse CDATA
void gXMLParserPrivate::fnc_ParseCDATA()
{
    const gToken &pToken=lexer.token();
    int state=0;
    int corclose=0;
    gNodeTag *tag=NULL;
    bool bParsedTag=false;
    while(state!=15 && !bCancel)
    {

        lexer.getToken();
        switch(state)
        {
        case 0: //expecting CDATA
            switch(pToken.tokenID1)
            {
            case TOKEN_TYPE_IDENT:
                if(pToken.tokenString=="CDATA")
                   state=1;
                else
                {
                    fnc_WriteError("Unexpected error, expecting CDATA but got");
                    state=15;
                }
                break;
            case TOKEN_TYPE_ENDOF_FILE:
                state=15;//reached end of script
                fnc_WriteError("Unexpected end of script");
                break;
            case TOKEN_TYPE_ERROR:
                //ignore crap
                break;
            default:

                fnc_WriteError("Unexpected error, expecting CDATA but got");
                state=15;

            break;
            }
            break;
        case 1:
            //Expecting [
            switch(pToken.tokenID1)
            {
            case TOKEN_TYPE_COROPEN:
                state=2;
                break;
            case TOKEN_TYPE_ENDOF_FILE:
                state=15;//reached end of script
                fnc_WriteError("Unexpected end of script");
                break;
            case TOKEN_TYPE_ERROR:
                //ignore crap
                break;
            default:

                fnc_WriteError("Unexpected error, expecting [ but got");
                state=15;

            break;
            }
            break;
          case 2:
            //Expecting data
            switch(pToken.tokenID1)
            {
            case TOKEN_TYPE_LESSTHAN:
                fnc_ParseTag();//Parse a tag
                bParsedTag=true;

                break;
            case TOKEN_TYPE_ENDOF_FILE:
                state=15;//reached end of script
                fnc_WriteError("Unexpected end of script");
                break;
            case TOKEN_TYPE_ERROR:
                //ignore crap
                break;
            case TOKEN_TYPE_CORCLOSE:
                corclose++;
                if(corclose==2)state=3;//ending CDATA
                break;
            default: //unknown data
                lexer.stepBack();
                tag=GetNodeTag();
                if(tag->bHasClosingTag==false && tag->dwType!=TAG_NODE_SINGLE && bParsedTag)

                    fnc_IgnoreAndSave("<",1);

                else
                    fnc_IgnoreAndSave("]",1);
                bParsedTag=false;
                corclose=0;
                break;
            }
            break;
        case 3:
            switch(pToken.tokenID1)
            {
            case TOKEN_TYPE_ENDOF_FILE:
                state=15;//reached end of script
                fnc_WriteError("Unexpected end of script");
                break;
            case TOKEN_TYPE_ERROR:
                //ignore crap
                break;
            case TOKEN_TYPE_GREATTHAN://finish CDATA
                state=15;
                break;
            }
            break;
        }

    }
}

void gXMLParserPrivate::fnc_IgnoreAndSave(char *iB,int size)
{
    gNodeTag *node;
    gString val;
    gu32 isize=0;
    char *buffer;
    int i;
    bool bDone=false;

    buffer=lexer.scriptAtPos(lexer.position());
    while(!bDone && lexer.currentChar()!=-1 && !bCancel)//Zero means end of file
    {
        for(i=0;i<size;i++)
        {
            if(lexer.currentChar()==iB[i])
                bDone=true;
        }
        if(!bDone)
        {
            lexer.nextChar();
            isize++;
        }


    }
    val.setString(buffer,isize);
    node=fnc_CreateNodeTag(0,val.ascii(),TAG_NODE_DATA);
    parseTree.addNode(node,CleanNodeTag);

}

//
void gXMLParserPrivate::fnc_JumpToFinalTag()
{
    int state=0;
 //   const gToken *tToken=0;
    const gToken &pToken=lexer.token();
    gNodeTag *tag;
     tag=GetNodeTag();
    while(state!=15 && !bCancel)
    {
        lexer.getToken();
        if(pToken.tokenID1==TOKEN_TYPE_ENDOF_FILE)
            state=15;
        switch(state)
        {
        case 0://
            if(pToken.tokenID1==TOKEN_TYPE_LESSTHAN)
            {
                state=1;
            }

            break;
        case 1:
            if(pToken.tokenID1==TOKEN_TYPE_DIAGONAL)
                state=2;

            else
                state=0;
            break;
        case 2:

            if(pToken.tokenID1==TOKEN_TYPE_IDENT)
            {
                if(tag->TagName==pToken.tokenString)
                    state=3;
                else
                    state=0;
            }
            break;
        case 3:
            if(pToken.tokenID1==TOKEN_TYPE_GREATTHAN)
            {
                tag->bHasClosingTag=true;
                parseTree.gotoFather();
                state=15;
            }
            else
                state=0;
            break;
        }


    }
}

gNodeTag *gXMLParserPrivate::fnc_CreateNodeTag(int iTag,const char *sName, gu32 dwType)
{
    gNodeTag *tag,*parent;

    tag=new gNodeTag;
    //set all idents to lowercase
    if(parseTree.isEmpty())
        parent=0;
    else
        parent=GetNodeTag();
    tag->Create(iTag,sName,dwType,lexer.column(),lexer.line(),parent);
    m_tagCount++;
    return tag;
}
