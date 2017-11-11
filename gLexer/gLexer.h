/***************************************GLEXER*********************************************************/
/*              FILE: glexer.h
                DESCRIPTION: Basic Lexer Engine
                AUTHOR: Daniel Rios
                WEBSITE: http://www.nikomaster.com
 */
#ifndef GLEXER_H
#define GLEXER_H
#include <gfw_global.h>
#include <gString/gString.h>
#include "gTokens.h"
namespace gfw
{
    struct SHARED_GFW gToken
    {
        gString tokenString;
        int tokenID1;
        int tokenID2;
        inline void clear()
        {
            tokenString.clear();
            tokenID1=-1;
            tokenID2=-1;
        }
        gToken():tokenID1(-1),tokenID2(-1)
        {

        }
        gToken(const gToken &other)
        {
            tokenString=other.tokenString;
            tokenID1=other.tokenID1;
            tokenID2=other.tokenID2;
        }

        gToken &operator=(const gToken &other)
        {
            tokenString=other.tokenString;
            tokenID1=other.tokenID1;
            tokenID2=other.tokenID2;
            return *this;
        }
    };

    class SHARED_GFW gLexer:public gBaseShared
    {
    public:
        gLexer();
        gLexer(const gLexer &other);
        ~gLexer();

        bool openScript(const gString &file);
        bool setScript(const gString &buffer);
        const gToken &getToken();//gets the next token
        const gToken &getTokenNV();//gets the next token counting negative numbers
        //returns reference to token element
        const gToken &token() const;
        const gString &tokenString() const;
        //now get the token but for writing and reading
        gToken &tokenAsRW();//read and write version of the token structure

        const gString &script() const;//get the script text
        const char *scriptAtPos(gu32 pos=0) const;//get the script at a position
        char *scriptAtPos(gu32 pos=0);//same as above but not const

        const wchar_t *scriptAtPosU(gu32 pos=0) const;
        wchar_t *scriptAtPosU(gu32 pos=0);
        gu32 scriptSize() const;//script size in characters

        int tokenID1() const;
        int tokenID2() const;

        void stepBack();//one token

        gLexer &operator=(const gLexer &other);

        char currentChar() const;//gets the current char from position
        wchar_t currentCharU() const;//gets the current char unicode version
        char nextChar();
        wchar_t nextCharU(); //unicode
        gu32 position() const;
        void setPosition(gu32 _position);
        //Current position on script
        gu32 line() const;
        gu32 column() const;
        void reset();

        void setTokenString(const gString &str);
        void setTokenID1(int id1);
        void setTokenID2(int id2);

        //return a substring
    };

}
#endif // GLEXER_H
