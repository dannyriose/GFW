#include "gLexerPrivate.h"
using namespace gfw;
#define ca GFW_PRIVATE_CAST(o,d,gLexerPrivate)
#define co GFW_PRIVATE_CAST(io,other.d,gLexerPrivate)
#define cn if(!d) return
#define acn gassert(d,"gLexer Object is empty")
gLexer::gLexer():gBaseShared(new gLexerPrivateFactory)
{
}

gLexer::gLexer(const gLexer &other):gBaseShared(other,new gLexerPrivateFactory)
{
}

gLexer::~gLexer()
{
}

bool gLexer::openScript(const gString &file)
{
   gFile f;

   clear();

   if(!f.open(file,gFile::GFILEAM_ACCESSREAD))
       return false;

   gLexerPrivate *o=new gLexerPrivate;
   o->data=f.readAllToString();
   o->size=o->data.size();
   d=o;
   return true;

}

char gLexer::currentChar() const
{
    acn;
    ca;
    char *s=o->data.ascii();
    if(o->position<o->size)
        return s[o->position];
    else
        return -1;
}

wchar_t gLexer::currentCharU() const
{
    acn;
    ca;
    wchar_t *s=o->data.unicode();
    if(o->position<o->size)
        return s[o->position];
    else
        return -1;
}
bool gLexer::setScript(const gString &buffer)
{
    ca;
    if(buffer.isEmpty())return false;
    clear();
    if(!o)
    {
        o=new gLexerPrivate;
        d=o;
    }
     o->data=buffer;
     o->size=buffer.size();
     return true;
}


const gToken &gLexer::getToken()
{
    acn;
    ca;
    short state=1;
    char c;
        //update that avoids a stack overflow
        //char temp[256]; replacing this
    o->token.clear();

    while(state!=-1)
    {
        c=o->getChar();
        if(c!=-125)
        {
            c=o->isAlpha(c);

            if(c!=65)
             c=o->isNumeric(c);
        }

        switch(c)
        {
        case 65:
            state=TOKEN_TYPE_IDENT;
            break;
        case 30:
            state=TOKEN_TYPE_NUMINT;
            break;
        case '('://Parentesis
            o->token.tokenID1=TOKEN_TYPE_PAROPEN;

            o->token.tokenString='(';
            state=-1;
            break;
        case ')':
            o->token.tokenID1=TOKEN_TYPE_PARCLOSE;

            o->token.tokenString=')';
            state=-1;
            break;
        case '[':
            o->token.tokenID1=TOKEN_TYPE_COROPEN;

            o->token.tokenString='[';
            state=-1;
            break;
        case ']':
            o->token.tokenID1=TOKEN_TYPE_CORCLOSE;

            o->token.tokenString=']';
            state=-1;
            break;
        case '{':
            o->token.tokenID1=TOKEN_TYPE_KEYOPEN;

            o->token.tokenString='{';
            state=-1;
            break;
        case '}':
            o->token.tokenID1=TOKEN_TYPE_KEYCLOSE;

            o->token.tokenString='}';
            state=-1;
            break;
        case ',':
            o->token.tokenID1=TOKEN_TYPE_COMA;

            o->token.tokenString=',';
            state=-1;
            break;
        case ';':
            o->token.tokenID1=TOKEN_TYPE_DOTCOMA;

            o->token.tokenString=';';
            state=-1;
            break;
        case ':':
            o->token.tokenID1=TOKEN_TYPE_TWODOTS;

            o->token.tokenString=':';
            state=-1;
            break;
      /*  case 'n':
            o->token.tokenID1=TOKEN_TYPE_EXCLAMATIONOPEN;

            o->token.tokenString='n';
            state=-1;
            break;*/

        case '"':
            o->token.tokenID1=TOKEN_TYPE_COMILLADBL;

            o->token.tokenString='"';
            state=-1;
            break;
        case 39:
            o->token.tokenID1=TOKEN_TYPE_COMILLASIN;

            o->token.tokenString=char(39);
            state=-1;
            break;
        case '$':
            o->token.tokenID1=TOKEN_TYPE_MONEY;

            o->token.tokenString='$';
            state=-1;
            break;
        case '%':
            o->token.tokenID1=TOKEN_TYPE_PERCENT;

            o->token.tokenString='%';
            state=-1;
            break;
        case '&':
            o->token.tokenID1=TOKEN_TYPE_ANDSIMBOL;

            o->token.tokenString='&';
            state=-1;
            break;
            break;
        case '|':
            o->token.tokenID1=TOKEN_TYPE_ORSIMBOL;

            o->token.tokenString='|';
            state=-1;
            break;
            break;
        case 47://
            o->token.tokenID1=TOKEN_TYPE_DIAGONAL;

            o->token.tokenString=char(47);
            state=-1;
            break;
        case 92:
            o->token.tokenID1=TOKEN_TYPE_INVDIAGONAL;

            o->token.tokenString=char(92);
            state=-1;
            break;

     /*   case 'n':
            o->token.tokenID1=TOKEN_TYPE_QUESTIONOPEN;

            o->token.tokenString='n';
            state=-1;
            break;*/
        case '?':
            o->token.tokenID1=TOKEN_TYPE_QUESTIONCLOSE;

            o->token.tokenString='?';
            state=-1;
            break;
        case '!':
            o->token.tokenID1=TOKEN_TYPE_EXCLAMATIONCLOSE;

            o->token.tokenString='!';
            state=-1;
            break;
        case 94:
            o->token.tokenID1=TOKEN_TYPE_POWEROPERATOR;

            o->token.tokenString=char(94);
            state=-1;
            break;
        case '.':
            o->token.tokenID1=TOKEN_TYPE_DOT;

            o->token.tokenString='.';
            state=-1;
            break;
        case '<':
            o->token.tokenID1=TOKEN_TYPE_LESSTHAN;

            o->token.tokenString='<';
            state=-1;
            break;
        case '>':
            o->token.tokenID1=TOKEN_TYPE_GREATTHAN;

            o->token.tokenString='>';
            state=-1;
            break;
        case '+':
            o->token.tokenID1=TOKEN_TYPE_PLUS;

            o->token.tokenString='+';
            state=-1;
            break;
        case '*':
            o->token.tokenID1=TOKEN_TYPE_ASTERISCO;

            o->token.tokenString='*';
            state=-1;
            break;
        case '_':
            o->token.tokenID1=TOKEN_TYPE_GUIONDOWN;

            o->token.tokenString='_';
            state=-1;
            break;
        case '-':
            o->token.tokenID1=TOKEN_TYPE_GUIONNORMAL;

            o->token.tokenString='-';
            state=-1;
            break;
        case '@':
            o->token.tokenID1=TOKEN_TYPE_ARROBA;

            o->token.tokenString='@';
            state=-1;
            break;
        case '~':
            o->token.tokenID1=TOKEN_TYPE_WEIRDSIMBOL;

            o->token.tokenString='~';
            state=-1;
            break;
        case '=':
            o->token.tokenID1=TOKEN_TYPE_EQUAL;

            o->token.tokenString='=';
            state=-1;
            break;
        case '#':
            o->token.tokenID1=TOKEN_TYPE_CAT;

            o->token.tokenString='#';
            state=-1;


            break;
        case ' ':
            break;
        case '\n':
            o->line++;//Nueva linea
            o->col=0;
            break;
        case 13:
            break;
        case 9:
            break;
        case -125:
            o->token.tokenID1=TOKEN_TYPE_ENDOF_FILE;
            state=-1;
            break;
        default:
            o->token.tokenID1=TOKEN_TYPE_ERROR;

            state=-1;
            break;
        };
        int start,end;
        char *data=o->data.ascii();
        char lastc;
        switch(state)
        {
        case TOKEN_TYPE_IDENT:
            o->stepBack();
            start=o->position;//get the start position
            c=o->getChar();
            end=0;
            //count the size of the characters
            while(((o->isAlpha(c)==65)||(o->isNumeric(c)==30))||(c=='_')||(c=='-'))
            {
                c=o->getChar();
                end++;
            }

            if(c!=-125)
                o->stepBack();
            o->token.tokenID1=TOKEN_TYPE_IDENT;
            //copy the string
            o->token.tokenString.setString(&data[start],end);
            state=-1;
            break;
        case TOKEN_TYPE_NUMINT:
            o->stepBack();
            start=o->position;
            c=o->getChar();
            end=0;
            while((o->isNumeric(c)==30))
            {
                c=o->getChar();
                end++;
                                //a++;
            };
            lastc=o->lastChar();
            if(c=='.')
            {
                end++;
                c=o->getChar();
                if(o->isNumeric(c)==30)
                {
                    while((o->isNumeric(c)==30))
                    {
                        c=o->getChar();
                        end++;
                    };
                    if(c!=-125)
                        o->stepBack();
                    o->token.tokenID1=TOKEN_TYPE_NUMFLOAT;
                    o->token.tokenString.setString(&data[start],end);
                    state=-1;
                }else
                {
                    o->token.tokenID1=TOKEN_TYPE_ERROR;
                    state=-1;
                    break;
                }
            }
            //End =1 and last char must be 0
            else if(c=='x' && end==1 && lastc=='0')//Hexadecimal style
            {
                //change start position end
                //0x445fd
                end++;
                start+=end;

                c=o->getChar();

                if(o->isHex(c))
                {
                    while((o->isHex(c)))
                    {
                        c=o->getChar();
                        end++;
                    };
                    o->stepBack();
                    o->token.tokenID1=TOKEN_TYPE_CSTYLEHEX;
                    o->token.tokenString.setString(&data[start],end);
                    state=-1;
                }else
                {
                    o->token.tokenID1=TOKEN_TYPE_ERROR;
                    state=-1;
                    break;
                }

            }
            else
            {
                if(c!=(-125))//Solamente cuando no se ha llegado al fin de archivo
                    o->stepBack();
                o->token.tokenID1=TOKEN_TYPE_NUMINT;
                o->token.tokenString.setString(&data[start],end);
                state=-1;
            };
            break;
        case 1:
            break;
        case -1:
            break;
        case -125:
            break;
        default:
            o->token.tokenID1=TOKEN_TYPE_ERROR;
            state=-1;
            break;
        }
    }

    return (const gToken &)o->token;
}


char gLexer::nextChar()
{
    char c;
    acn;
    ca;
    c=o->getChar();
    if(c=='\n')
        o->line++;
    return c;
}

wchar_t gLexer::nextCharU()
{
    wchar_t c;
    acn;
    ca;
    c=o->getCharU();

    if(c==char('\n'))
        o->line++;
    return c;
}

void gLexer::stepBack()
{
    acn;
    ca;

    o->position-=o->token.tokenString.size();
}


gu32 gLexer::position() const
{
    cn 0;
    ca;

    return o->position;
}

void gLexer::setPosition(gu32 _position)
{
    acn;
    ca;
    o->position=_position;
}

gu32 gLexer::line() const
{
    cn 0;
    ca;
    return o->line;
}


gu32 gLexer::column() const
{
    cn 0;
    ca;
    return o->col;
}

void gLexer::reset()
{
    cn;
    ca;

    o->position=0;
    o->col=0;
    o->line=0;
}



gLexer &gLexer::operator =(const gLexer &other)
{
    assign(other);
    return *this;
}


const gToken &gLexer::token() const
{
    acn;
    ca;

    return (const gToken &)o->token;
}

gToken &gLexer::tokenAsRW()
{
    acn;
    ca;
    return o->token;
}

const gString &gLexer::tokenString() const
{
    acn;
    ca;

    return (const gString &)o->token.tokenString;
}


int gLexer::tokenID1() const
{
    acn;
    ca;
    return o->token.tokenID1;
}


int gLexer::tokenID2() const
{
    acn;
    ca;
    return o->token.tokenID2;
}


const gString &gLexer::script() const
{
    acn;
    ca;

    return (const gString &)o->data;
}


const char *gLexer::scriptAtPos(gu32 pos) const
{
    acn;
    ca;
    gassert(pos<o->position,"Index pos out of range, function scriptAtPos");
    const char *s=(const char *)o->data.ascii();

    return &s[pos];
}


char *gLexer::scriptAtPos(gu32 pos)
{
    acn;
    ca;
    gassert(pos<o->size,"Index pos out of range, function scriptAtPos");
    char *s=o->data.ascii();

    return &s[pos];
}

const wchar_t *gLexer::scriptAtPosU(gu32 pos) const
{
    acn;
    ca;

    gassert(pos<o->position,"Index pos out of range, function scriptAtPosU");
    const wchar_t *s=(const wchar_t *)o->data.unicode();

    return &s[pos];
}


wchar_t *gLexer::scriptAtPosU(gu32 pos)
{
    acn;
    ca;

    gassert(pos<o->position,"Index pos out of range, function scriptAtPosU");
    wchar_t *s=o->data.unicode();

    return &s[pos];
}

void gLexer::setTokenString(const gString &str)
{
    acn;
    ca;
    o->token.tokenString=str;
}

void gLexer::setTokenID1(int id1)
{
    acn;
    ca;
    o->token.tokenID1=id1;
}

void gLexer::setTokenID2(int id2)
{
    acn;
    ca;
    o->token.tokenID2=id2;
}

const gToken &gLexer::getTokenNV(){
    //gets the token but takes into account negative numbers
    acn;
    ca;
    getToken();
    if(o->token.tokenID1==TOKEN_TYPE_GUIONNORMAL){
        getToken();
       if(o->token.tokenID1==TOKEN_TYPE_NUMINT || o->token.tokenID1==TOKEN_TYPE_NUMFLOAT)
       {
           o->token.tokenString=gString("-") + o->token.tokenString;
        }else{
            stepBack();
        }
     }
    return (const gToken &)o->token;
}
