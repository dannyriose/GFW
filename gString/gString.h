/***********************************GSTRING.H********************************************************/
/* FILE: gstring.h                                                                                  */
/* DESCRIPTION: A generic implementation of a flexible string class                                 */
#ifndef GSTRING_H
#define GSTRING_H
#include <gfw_global.h>
#include <gMemory/gBaseArray.h>
#include <gArray/gList.h>
//Define library exports if included
//define SHARED_LIBRARY if object is meant to be included inside a shared object library


#include <stdio.h>
#include <string>
//define U_QT for compatibility with QT QString
#ifdef U_QT
#include <QString>
#endif

//define GSTRING_ASCII to make all strings ASCII format as default
#ifdef GSTRING_ASCII
#define GSTRING_DEFAULT ASCII
#else
#define GSTRING_DEFAULT UTF
#endif
namespace gfw
{
    class gStringList;
    //! Utility class that represents strings.
    /** gString class includes functionality for handling ascii an unicode strings. It is light weight
      and easy to implement on any application as it includes support for other string types such as
      std::string,std::wstring and Qt String. It includes several functions for manipulation.
      This is one of the main core components of the G-Framework */
    class SHARED_GFW gString:public gBaseShared{

#if DEBUG
    private:
        //! Just for debuging purposes, it is a pointer to the string.
        char *_debg;//for debug purposes only
#endif
    public:
        //! Defines String formats
        enum GSTRING_FORMAT
        {
            //! Object contains the string in ascii format
            ASCII,
            //! Object contaisn the string in utf or wide char format
            UTF
        };
        //! Constructor
        gString();//Generic Constructor
        //! Copy constructor
        /** \param other: gString object to initialize object. */
        gString(const gString &other);//Copy constructor
        //! Copy constructor with option to initialize the object as reference.
        /** \param other: gString object to initialize object.
            \param bAsReference: set it true to initialize the object as reference */
        gString(const gString &other, bool bShared);
        gString(const char *szstring);//With null terminated string
        gString(const char *szString, bool bShared);
        gString(const wchar_t *ustring);
        gString(const wchar_t *ustring, bool bShared);
        gString(const std::string &stdstring);//support for str string too
        gString(const std::wstring &wstdstring);//support for wide std string too
        ~gString();
    #ifdef U_QT
        gString(const QString &qtString);//support for QT String if enabled
    #endif

        //
        void setString(const char *szstring,gu32 lenght=0);
        void setString(const gString &other,gu32 length=0);
        void setString(const wchar_t *ustring, gu32 lenght=0);
        void setString(const std::string &stdstring, gu32 lenght=0);
        void setString(const std::wstring &wstdstring,gu32 length=0);
        void setString(char c);
        void setString(wchar_t c);
    #ifdef U_QT
        void setString(const QString &qtString,gu32 lenght=0);
        QString qtString() const; //returns as a qt string
    #endif
        char *ascii() const;//Returns the pointer as  ASCII
        wchar_t *unicode() const;//returns the pointer as UNICODE

        //size of string
        gu32 size() const;
        //size in bytes
        gu32 byteSize() const;
        //Handy operators
        gString &operator=(const gString &other);
        gString &operator=(const char *ascii);
        gString &operator=(const wchar_t *ustring);
        gString &operator=(char c);
        gString &operator=(wchar_t c);
        gString &operator=(const std::string &stdstring);
        gString &operator=(const std::wstring &wstdstring);
    #ifdef U_QT
        gString &operator=(const QString &qtString);
    #endif
        //More handy operators
        gString &operator+=(const gString &other);
        gString &operator+=(const char *ascii);
        gString &operator+=(const wchar_t *ustring);
        gString &operator+=(char c);
        gString &operator+=(wchar_t c);
        gString &operator+=(const std::string &stdstring);
        gString &operator+=(const std::wstring &wstdstring);
    #ifdef U_QT
        gString &operator+=(const QString &qtString);
    #endif

        gString operator + (const gString &other) const;
        gString operator + (const char *ascii) const;
        gString operator + (const wchar_t *ustring) const;
        gString operator + (const std::string &stdstring) const;
        gString operator + (const std::wstring &wstdstring) const;
    #ifdef U_QT
        gString operator + (const QString &qtString) const;
    #endif
        //char operator [](int index) const;
        //char &operator [](int index);
        //And even more handy operators
        operator const char *() const;
        operator const wchar_t *() const;

        //Allocation routines
        bool alloc(gu32 _size, int _format);

        //Numbers
        static gString number(int value, int format=GSTRING_DEFAULT);
        static gString number(double value,char format='g',int precision=6,
                              int strFormat=GSTRING_DEFAULT);
        static gString number(gu32 value,int format=GSTRING_DEFAULT);
        static gString number(gs64 value,int format=GSTRING_DEFAULT);
        static gString number(gu64 value,int format=GSTRING_DEFAULT);
        //Conversion to numbers
        gs32 toInt(bool *ok=0, int base=10) const;
        gu32 toUInt(bool *ok=0, int base=10) const;
        gf32 toFloat(bool *ok=0) const;
        gf64 toDouble(bool *ok=0) const;
        gs64 toInt64(bool *ok=0, int base=10) const;
        gu64 toUInt64(bool *ok=0, int base=10) const;
        //Some other interesting functions
        int format() const;
        //! Defines if object is passed as reference when using operator =, if set ownership of data passes to assigned string
        bool isNumeric() const;//Returns true if data is numeric: integer, or floating point
        bool isInteger() const;//Retunrs true if data is signed integer
        bool isUInteger() const;//Returns true if data unsigned integer
        gString toAscii() const;//Returns a copy of the string in ascii format
        gString toUnicode() const;//Returns a copy of the string in unsicode format
        void thisToAscii();//Converts this object to ascii
        void thisToUnicode();//Converts this object to unicode

        gString toLowCase() const;//returns a copy of the string lowcase
        gString toUpperCase() const;//returns a copy of the string uppercase
        void thisToLowCase();
        void thisToUpperCase();
        std::string toStdString() const;
        std::wstring towStdString() const;

        void concatToThis(const char *src,gu32 lenght=0);
        void concatToThis(const wchar_t *src,gu32 lenght=0);
        void concatToThis(const gString &other, gu32 lenght=0);
        void concatToThis(const std::string &stdstring,gu32 length=0);
        void concatToThis(const std::wstring &wstdstring,gu32 length=0);
#ifdef U_QT
        void concatToThis(const QString &qtString,gu32 lenght=0);
#endif

        //comparison
        bool operator == (const char *s) const;
        bool operator == (const wchar_t *s) const;
        bool operator == (const gString &other) const;
        bool operator == (const std::string &stdstring) const;
        bool operator == (const std::wstring &wstdstring) const;
#ifdef U_QT
        bool operator == (const QString &qtString) const;
        bool operator != (const QString &qtString) const;
#endif

        bool operator != (const char *s) const;
        bool operator != (const wchar_t *s) const;
        bool operator != (const gString &other) const;
        bool operator != (const std::string &stdstring) const;
        bool operator != (const std::wstring &wstdstring) const;
        //Substrings and search
        gString subString(gu32 pos) const;
        gString subString(gu32 pos,gu32 lenght) const;
        int strPos(const gString &other,int pos) const;
        gString replace(const gString &replaceWhat, const gString &replacement) const;
        gString replace(const gByteArray &charList, const gByteArray &replacement) const;
        gStringList split(const gString &delimiter) const;
        void appendUint16AsHex(gu16 value);
        //! Gets the hash value of the string
        gu32 hash() const;
        //search todo

        //static function utilities
        static char *clone_ascii(const char *src);
        static wchar_t *clone_unicode(const wchar_t *src);
        //Concatenation
        static char *cat_ascii(const char *str1,const char *str2);
        static void catto_ascii(char *dest,const char *source);
        static void catto_unicode(wchar_t *dest,const wchar_t *source);
        static wchar_t *cat_unicode(const wchar_t *str1,const char *str2);
        //Freeing memory
        static void free_string(void *str);
        //Additional constructors
        static char *convertToAscii(const wchar_t *ustring);
        static wchar_t *convertToUnicode(const char *astring);
        //! Static function creates a gString object from a byte array.
        /** \param ba: gByteArray object containing the array.
            \return a new gString object. */
        static gString fromByteArray(const gByteArray &ba);
        //more utility functions
        //! Static utility function that creates a string in hex format from a short integer.
        /** \param source: Short integer to be converted to string.
            \return a new gString object containing 'source' in hex format. */
        static gString uint16toHex(gu16 source);
        //! Static utility function that creates a string in hex format from an integer.
        /** \param source: Integeger to be converted to string.
            \return a new gString object containing 'source' in hex format */
        static gString uint32toHex(gu32 source);
        //! Static utility function that converts a string in hex format to a short integer.
        /** \param src: gString object containing the string in hex format.
            \return value of hex string. */
        static gu16 fromHexToUint16(const gString &src);
        //! Static utility function that converts a string in hex format to an array of short integers.
        /** \param src: gString object containing the string in hex format. It is supposed the string
          contains several hex values in parts of four. For example the string fff456ad2356 would contain
          three short integers.
            \return an array of short integers. It returns an empty object if failed. */
        static gUShortArray fromHexToUint16Array(const gString &src);

        //More utilites
        //! Static utility function that encodes all html entities contained in the string.
        /** \param src: gString object containing the string with html entities.
            \return new gString object with the encoded string. */
        static gString htmlEntitiesEncode(const gString &src);

    };
    typedef gPair<gString,gString> gStringPair;
}
#endif // GSTRING_H
