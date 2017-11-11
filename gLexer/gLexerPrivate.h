#ifndef GLEXERPRIVATE_H
#define GLEXERPRIVATE_H
#include "gLexer.h"
#include <gFile/gFile.h>
namespace gfw{

struct gLexerPrivate:public gSharedObject
{
    gString data;//script data
    gu32 position;
    gu32 line;
    gu32 col;
    gToken token;
    gu32 size;
    gLexerPrivate():position(0),line(0),col(0),size(0)
    {

    }

    //help functions
    char getChar()
    {
        if(position==size) return -125;
        char *s=data.ascii();
        char c;

        c=s[position];
        position++;
        col++;
        return c;
    }
    char lastChar()
    {
        if(position==size) return -125;
        char *s=data.ascii();
        char c;
        if(position==0)return s[position];

        c=s[position-1];
        return c;
    }

    wchar_t getCharU()
    {
        if(position==size) return -125;
        wchar_t *s=data.unicode();
        wchar_t c;

        c=s[position];
        position++;
        col++;
        return c;
    }

    void stepBack()
    {
        if(int(position)>0)
            position--;
    }

    int isAlpha(int c)
    {
        int r;
        if(c==-125) return c;
        r=isalpha((unsigned char)c);
        if(r)return 65;
        else return c;
    }

    int isNumeric(int c)
    {
        int r;
        if(c==-125) return c;
        r=isdigit((unsigned char)c);
        if(r)return 30;
        else return c;
    }
    bool isHex(int c)
    {
        bool r=false;
        if(c==-125) return c;
        r=(c=='A' || c=='B' || c=='C' || c=='D' || c=='E' || c=='F')
                || (c=='a' || c=='b' || c=='c' || c=='d' || c=='e' || c=='f');
        r= r || (isNumeric(c)==30);
        return r;
    }
    virtual void copy(const gSharedObject *other){
        GFW_PRIVATE_CAST(io,other,gLexerPrivate);
        col=io->col;
        data=io->data;
        line=io->line;
        position=io->position;
        size=io->size;
        token=io->token;
    }
    ~gLexerPrivate(){

    }


};

G_SHAREDOBJECT_FACTORY(gLexerPrivateFactory,gLexerPrivate);
}
#endif // GLEXERPRIVATE_H
