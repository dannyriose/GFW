#include "impl/gStringPrivate.h"
using namespace gfw;
//Handy lazy macros
#define ca GFW_PRIVATE_CAST(o,d,gStringPrivate)
#define co GFW_PRIVATE_CAST(io,other.d,gStringPrivate)
#define cs d=new gStringPrivate
#define acn assert(d)
#define cn if(!d) return
gString::gString():gBaseShared(new gStringPrivateFactory){

}


gString::gString(const gString &other):gBaseShared(other,new gStringPrivateFactory){
#ifdef DEBUG
    GFW_PRIVATE_CAST(o,d,gStringPrivate);
    _debg=o->data;
#endif
}

gString::gString(const gString &other,bool bShared):gBaseShared(other,new gStringPrivateFactory){
  if(d)
      setShared(bShared);
}

gString::gString(const char *szstring):gBaseShared(new gStringPrivateFactory){
    setString(szstring);
}

gString::gString(const char *szstring,bool bShared):gBaseShared(new gStringPrivateFactory){
    setString(szstring);
    setShared(bShared);
}



gString::gString(const wchar_t *ustring):gBaseShared(new gStringPrivateFactory){
    setString(ustring);
}

gString::gString(const wchar_t *ustring,bool bShared):gBaseShared(new gStringPrivateFactory){
    setString(ustring);
    setShared(bShared);

}
gString::gString(const std::string &stdstring):gBaseShared(new gStringPrivateFactory){
    setString(stdstring);
}
gString::gString(const std::wstring &wstdstring):gBaseShared(new gStringPrivateFactory){
    setString(wstdstring);
}

#ifdef U_QT
gString::gString(const QString &qtString):gBaseShared(new gStringPrivateFactory){
    setString(qtString);
}
#endif

//Memory allocation
gString::~gString(){

}

bool gString::alloc(gu32 _size, int _format)
{
    clear();
    GFW_PRIVATE_CREATE(o,d,gStringPrivate);
    return o->alloc(_size,_format);
}

void gString::setString(const char *szstring, gu32 lenght)
{
    gu32 l;
    //gassert(szstring,"String parameter passed is null");
    if(!szstring)return;
    if(!lenght)
        l=strlen(szstring);
    else
        l=lenght;

    if(alloc(l,ASCII))
    {
        ca;
        memcpy(o->data,szstring,o->sizebytes);
    }
}

void gString::setString(const wchar_t *ustring,gu32 lenght)
{
    gu32 l;
    //gassert(ustring,"unicode string passed is null");
    if(!ustring)return;
    if(!lenght)
        l=wcslen(ustring);
    else
        l=lenght;

    if(alloc(l,UTF))
    {
        ca;
        memcpy(o->data,ustring,o->sizebytes);
    }

}

void gString::setString(const gString &other, gu32 length)
{
    co;
    //gassert(io,"Object gstring other is empty");
    if(!io)return;
    if(d == (void *)io){ //can't do this
        return;
    }
    gu32 _size;

    if(!length)
        _size=io->size;
    else
    {
        gassert(length<=io->size,"setString, parameter lenght bigger then source string");
        _size=length;
    }
    if(alloc(io->size,io->format))
    {
        ca;
        memcpy(o->data,io->data,io->sizebytes);
    }
}
void gString::setString(const std::string &stdstring,gu32 lenght){
    setString(stdstring.c_str(),lenght);
}
void gString::setString(const std::wstring &wstdstring, gu32 length){
    setString(wstdstring.c_str(),length);
}

void gString::setString(char c)
{
    if(alloc(1,ASCII))
    {
        ca;
        o->data[0]=c;
    }
}

void gString::setString(wchar_t c)
{
    wchar_t *w;
    if(alloc(2,UTF))
    {
        ca;
        w=(wchar_t *)o->data;
        w[0]=c;
        w[1]=0;
    }
}

#ifdef U_QT
//finally adding this section
void gString::setString(const QString &qtString, gu32 lenght)
{
    if(qtString.isEmpty())return;
    if(GSTRING_DEFAULT==ASCII)
    {
        setString(qtString.toAscii().data(),lenght);
    }
    else
    {
        gu32 l;
        if(!lenght)
            l=qtString.size();
        else
            l=lenght;

        if(l>0){
            if(alloc(l,UTF))
            {
                ca;
                qtString.toWCharArray((wchar_t *)o->data);
            }
        }
    }

}

QString gString::qtString() const
{
    acn;
    ca;
    if(o->format==ASCII)
        return QString::fromAscii(o->data);
    else
        return QString::fromWCharArray((wchar_t *)o->data);
}

//taking advantage
gString &gString::operator =(const QString &qtString)
{
    setString(qtString);
    return *this;
}

gString &gString::operator +=(const QString &qtString)
{
    //easy?
    concatToThis(qtString);
    return *this;

}

gString gString::operator + (const QString &qtString) const
{
    gString r(*this);
    r+=qtString;
    r.setShared(true);

    return gString(r);
}

void gString::concatToThis(const QString &qtString, gu32 lenght)
{
    ca;
    if(qtString.isEmpty())return;
    if(!o)
    {
        setString(qtString);
        return;
    }
    if(o->format==ASCII)
    {
        concatToThis(qtString.toAscii().data(),lenght);
    }
    else
    {
        gu32 oldsize,oldtsize,ssize;
        char *oldpointer;
        wchar_t *upointer;
        oldsize=o->size;
        oldtsize=o->sizebytes;
        oldpointer=o->data;
        if(!lenght)
            ssize=qtString.size();
        else
            ssize=lenght;

        o->size+=ssize;
        o->sizebytes=o->size * sizeof(wchar_t);

        o->data=(char *)realloc(o->data,o->sizebytes+sizeof(wchar_t));
        if(o->data)
        {
            upointer=(wchar_t *)o->data;
            //memcpy(&upointer[oldsize],src,ssize * sizeof(wchar_t));
            qtString.toWCharArray(&upointer[oldsize]);
            upointer[o->size]=0;//null terminator
        }
        else
        {
            o->size=oldsize;
            o->sizebytes=oldtsize;
            o->data=oldpointer;
        }
    }

}

#endif

char *gString::ascii() const
{
    if(!d)return 0;
    ca;

    gassert(o->format==ASCII,"Function gString::ascii(),current format is not ASCII");

    return o->data;
}


wchar_t *gString::unicode() const
{
    if(!d)return 0;
    ca;
    gassert(o->format==UTF,"Function gString::unicode(), current format is not UTF");

    return (wchar_t *)o->data;
}

gu32 gString::size() const
{
    cn 0;
    ca;

    return o->size;
}

gu32 gString::byteSize() const
{
    cn 0;
    ca;

    return o->sizebytes;
}


gString &gString::operator =(const gString &other)
{
    assign(other);
#ifdef DEBUG
    GFW_PRIVATE_CAST(io,d,gStringPrivate);
    _debg=io->data;
#endif   
    return *this;
}


gString &gString::operator =(const char *str)
{
    setString(str);
    return *this;
}


gString &gString::operator =(const wchar_t *str)
{
    setString(str);
    return *this;
}
gString &gString::operator = (char c)
{
    setString(c);
    return *this;
}
gString &gString::operator =(wchar_t c)
{
    setString(c);
    return *this;
}
gString &gString::operator = (const std::string &stdstring){
    setString(stdstring);
    return *this;
}
gString &gString::operator = (const std::wstring &wstdstring){
    setString(wstdstring);
    return *this;
}

void gString::concatToThis(const char *src,gu32 lenght)
{
    ca;
    gassert(src,"function gString::concatToThis(const char *src). Parameter src is null");
    if(!o)
    {
        setString(src,lenght);
        return;
    }
    if(o->format!=ASCII){
       gString un=src;
       un.thisToUnicode();
       concatToThis(un.unicode(),un.size());
       return;
    }
    gu32 oldsize,oldtsize,ssize;
    char *oldpointer;

    oldsize=o->size;
    oldtsize=o->sizebytes;
    oldpointer=o->data;
    if(!lenght)
        ssize=strlen(src);
    else
        ssize=lenght;

    o->size+=ssize;
    o->sizebytes=o->size;

    o->data=(char *)realloc(o->data,o->size+1);
    if(o->data)
    {
        memcpy(&o->data[oldsize],src,ssize);
        o->data[o->size]=0;//null terminator
    }
    else
    {
        o->size=oldsize;
        o->sizebytes=oldtsize;
        o->data=oldpointer;
    }
#ifdef DEBUG
    _debg=o->data;
#endif

}


void gString::concatToThis(const wchar_t *src,gu32 lenght)
{
    ca;
    gassert(src,"function gString::concatToThis(const wchar_t *src). Parameter src is null");

    if(!o)
    {
        setString(src,lenght);
        return;
    }
    if(o->format!=UTF){
        gString as=src;
        as.thisToAscii();
        concatToThis(as.ascii(),as.size());
        return;
    }
    gu32 oldsize,oldtsize,ssize;
    char *oldpointer;
    wchar_t *upointer;
    oldsize=o->size;
    oldtsize=o->sizebytes;
    oldpointer=o->data;
    if(!lenght)
        ssize=wcslen(src);
    else
        ssize=lenght;

    o->size+=ssize;
    o->sizebytes=o->size * sizeof(wchar_t);

    o->data=(char *)realloc(o->data,o->sizebytes+sizeof(wchar_t));
    if(o->data)
    {
        upointer=(wchar_t *)o->data;
        memcpy(&upointer[oldsize],src,ssize * sizeof(wchar_t));
        upointer[o->size]=0;//null terminator
    }
    else
    {
        o->size=oldsize;
        o->sizebytes=oldtsize;
        o->data=oldpointer;
    }
#ifdef DEBUG
    _debg=o->data;
#endif

}


void gString::concatToThis(const gString &other,gu32 lenght)
{
    if(other.isEmpty())return;
    ca;
    //gassert(!other.isEmpty(),"function gString::concatToThis(const wchar_t *src). Parameter other is an empty string");

    if(!o)
    {
        setString(other,lenght);
        return;
    }
    gu32 oldsize,oldtsize,ssize;
    char *oldpointer;
    char *tpointer;
    wchar_t *wctpointer,*upointer;
    gString tmp,ntemp;
    const gString *temp;
    co;
    //check if by any chance both objects share data.
    if(d == other.d){
        //not the same object
        if(this!=&other){//check if both are different objects
            //shared private members between objects
            //simple solution
            *this = *this + other;//this will make a string data
            return;
        }
        //same object, then we make a small trick
        //set a temporary object and copy the string there
        //since alloc clears previous data to avoid issues we use a temporal string
        tmp.setString(other);//this creates a new string
        temp=(const gString *)&tmp;//pass the pointer
        io=(gStringPrivate *)tmp.d;
    }else{
        temp=&other;
    }
    oldsize=o->size;
    oldtsize=o->sizebytes;
    oldpointer=o->data;
    if(!lenght)
        o->size+=io->size;
    else
    {
        gassert(lenght<=io->size,"Source string gString::concatTothis, parameter lenght bigger than string");
        o->size+=lenght;

    }
    //verify if by any chance the 'other' string contains the same private member
    //if so we must deal with this

    switch(o->format)
    {
    case ASCII:
        o->sizebytes=o->size;
        o->data=(char *)realloc(o->data,o->size + 1);
        if(io->format==UTF)
        {
            ntemp=temp->toAscii();
            tpointer=ntemp.ascii();
        }
        else
            tpointer=temp->ascii();
        if(o->data)
        {
            memcpy(&o->data[oldsize],tpointer,io->size);
            o->data[o->size]=0;
        }
        else
        {
            o->size=oldsize;
            o->sizebytes=oldtsize;
            o->data=oldpointer;
        }
#ifdef DEBUG
    _debg=o->data;
#endif
        break;
    case UTF:
        o->sizebytes=o->size * sizeof(wchar_t);

        o->data=(char *)realloc(o->data,o->sizebytes+sizeof(wchar_t));
        if(io->format==ASCII)
        {
            ntemp=temp->toUnicode();
            wctpointer=ntemp.unicode();
        }
        else
            wctpointer=temp->unicode();
        if(o->data)
        {
            upointer=(wchar_t *)o->data;
            memcpy(&upointer[oldsize],wctpointer,io->size * sizeof(wchar_t));
            upointer[o->size]=0;//null terminator
        }
        else
        {
            o->size=oldsize;
            o->sizebytes=oldtsize;
            o->data=oldpointer;
        }
#ifdef DEBUG
    _debg=o->data;
#endif

    }


}
void gString::concatToThis(const std::string &stdstring, gu32 length){
    concatToThis(stdstring.c_str(),length);
}
void gString::concatToThis(const std::wstring &wstdstring, gu32 length){
    concatToThis(wstdstring.c_str(),length);
}

gString &gString::operator +=(const gString &other)
{
   concatToThis(other);

   return *this;

}


gString &gString::operator +=(const char *str)
{
   concatToThis(str);
   return *this;
}

gString &gString::operator +=(const wchar_t *str)
{
    concatToThis(str);
    return *this;
}

gString &gString::operator +=(char c)
{
    char s[2];
    s[0]=c;
    s[1]=0;
    concatToThis(s,1);
    return *this;
}

gString &gString::operator +=(wchar_t wc)
{
    wchar_t s[2];
    s[0]=wc;
    s[1]=0;
    concatToThis(s,1);
    return *this;
}
gString &gString::operator +=(const std::string &stdstring){
    concatToThis(stdstring);
    return *this;
}
gString &gString::operator +=(const std::wstring &wstdstring){
    concatToThis(wstdstring);
    return *this;
}

gString gString::operator + (const char *str) const
{
    gString other;

    other.setString(*this);
    other.concatToThis(str);
    other.setShared(true);

    return gString(other);

}

gString gString::operator +(const wchar_t *str) const
{
    gString other;

    other.setString(*this);
    other.concatToThis(str);
    other.setShared(true);

    return gString(other);
}

gString gString::operator +(const gString &other) const
{
    gString r;
    r.setString(*this);
    r.concatToThis(other);
    r.setShared(true);

    return gString(r);
}
gString gString::operator + (const std::string &stdstring) const{
    gString r;
    r.setString(*this);
    r.concatToThis(stdstring);
    r.setShared(true);
    return gString(r);
}
gString gString::operator + (const std::wstring &wstdstring) const{
    gString r;
    r.setString(*this);
    r.concatToThis(wstdstring);
    r.setShared(true);
    return gString(r);
}
gString::operator const char *() const
{
    cn 0;
    ca;
    gassert(o->format==ASCII,"operator const char *(), string format is unicode");

    return (const char *)o->data;
}

gString::operator const wchar_t *() const
{
    cn 0;
    ca;
    gassert(o->format==UTF,"operator const wchar_t *(), string format is ASCII");

    return (const wchar_t *)o->data;
}

//numbers

gString gString::number(int value,int format)
{
    gString r;
    gString a;
    char *as;
    wchar_t *wc;
    a.alloc(20,format);

    if(format==ASCII)
    {
        as=a.ascii();
        sprintf(as,"%d",value);
        r.setString(as);
    }
    else if(format==UTF)
    {
        wc=a.unicode();
        swprintf(wc,20,L"%d",value);
        r.setString(wc);

    }
    r.setShared(true);

    return gString(r);

}


gString gString::number(gu32 value,int format)
{
    gString r;
    gString a;
    char *as;
    wchar_t *wc;
    a.alloc(20,format);

    if(format==ASCII)
    {
        as=a.ascii();
        sprintf(as,"%u",value);
        r.setString(as);
    }
    else if(format==UTF)
    {
        wc=a.unicode();
        swprintf(wc,20,L"%u",value);
        r.setString(wc);

    }
    r.setShared(true);

    return gString(r);

}


gString gString::number(gs64 value, int format)
{
    gString r;
    gString a;
    char *as;
    wchar_t *wc;
    a.alloc(40,format);

    if(format==ASCII)
    {
        as=a.ascii();
        sprintf(as,"%lld",value);
        r.setString(as);
    }
    else if(format==UTF)
    {
        wc=a.unicode();
        swprintf(wc,40,L"%lld",value);
        r.setString(wc);

    }
    r.setShared(true);

    return gString(r);
}

gString gString::number(gu64 value, int format)
{
    gString r;
    gString a;
    char *as;
    wchar_t *wc;
    a.alloc(40,format);

    if(format==ASCII)
    {
        as=a.ascii();
        sprintf(as,"%llu",value);
        r.setString(as);
    }
    else if(format==UTF)
    {
        wc=a.unicode();
        swprintf(wc,40,L"%llu",value);
        r.setString(wc);

    }
    r.setShared(true);

    return gString(r);
}


gString gString::number(double value, char format, int precision, int strFormat)
{
    gString a,f,r;
    char *as,*frm;
    wchar_t *wc,*wfrm;

    a.alloc(80,strFormat);


    f.alloc(10,strFormat);

    if(strFormat==ASCII)
    {
        frm=f.ascii();
        sprintf(frm,"%c.%d%c",37,precision,format);
        as=a.ascii();
        sprintf(as,frm,value);
        r.setString(as);
    }
    else
    {
        wfrm=f.unicode();
        swprintf(wfrm,80,L"%c.%d%c",37,precision,format);
        wc=a.unicode();
        swprintf(wc,80,wfrm,value);
        r.setString(wc);
    }

    r.setShared(true);

    return gString(r);

}

//conversion

gs32 gString::toInt(bool *ok,int base) const
{
    bool numeric;
    gs32 ret=0;
    if(!d)return 0;
    ca;
    numeric=isInteger();
    if(ok)
        *ok=numeric;

    if(numeric)
    {
        if(o->format==ASCII)
            ret=(gs32)strtol(o->data,0,base);
        else
            ret=(gs32)wcstol((wchar_t *)o->data,0,base);
    }

    return ret;
}

gu32 gString::toUInt(bool *ok,int base) const
{
    bool numeric;
    gu32 ret=0;
    if(!d)return 0;
    ca;
    numeric=isUInteger();
    if(ok)
        *ok=numeric;

    if(numeric)
    {
        if(o->format==ASCII)
            ret=(gu32)strtoul(o->data,0,base);
        else
            ret=(gu32)wcstoul((wchar_t *)o->data,0,base);
    }

    return ret;
}

gf64 gString::toDouble(bool *ok) const
{
    bool numeric;
    gf64 ret=0;
    if(!d)return 0;
    ca;
    numeric=isNumeric();
    if(ok)
        *ok=numeric;

    if(numeric)
    {
        if(o->format==ASCII)
            ret=(gf64)strtod(o->data,0);
        else
            ret=(gf64)wcstod((wchar_t *)o->data,0);
    }

    return ret;
}

gf32 gString::toFloat(bool *ok) const
{
    bool numeric;
    gf32 ret=0;
    if(!d)return 0;
    ca;
    numeric=isNumeric();
    if(ok)
        *ok=numeric;

    if(numeric)
    {
        if(o->format==ASCII)
            ret=(gf32)strtod(o->data,0);
        else
            ret=(gf32)wcstod((wchar_t *)o->data,0);
    }

    return ret;
}

gs64 gString::toInt64(bool *ok,int base) const
{
    bool numeric;
    gs64 ret=0;
    if(!d)return 0;
    ca;
    numeric=isUInteger();
    if(ok)
        *ok=numeric;

    if(numeric)
    {
        if(o->format==ASCII)
#ifdef WIN32
            ret=_strtoi64(o->data,0,base);
#else
            ret=strtoll(o->data,0,base);
#endif
        else
#ifdef WIN32
            ret=_wcstoi64((wchar_t *)o->data,0,base);
#elif __gnu_linux__
            ret=wcstoll((wchar_t *)o->data,0,base);
#endif
    }

    return ret;
}

gu64 gString::toUInt64(bool *ok,int base) const
{
    bool numeric;
    gu64 ret=0;
    if(!d)return 0;
    ca;
    numeric=isUInteger();
    if(ok)
        *ok=numeric;

    if(numeric)
    {
        if(o->format==ASCII)
#ifdef WIN32
            ret=_strtoi64(o->data,0,base);
#else
            ret=strtoull(o->data,0,base);
#endif
        else
#ifdef WIN32
            ret=_wcstoui64((wchar_t *)o->data,0,base);
#elif __gnu_linux__
            ret=wcstoull((wchar_t*)o->data,0,base);
#endif
    }

    return ret;
}


int gString::format() const
{
    acn;

    ca;

    return o->format;
}


bool gString::isNumeric() const
{
    if(!d)return false;
    ca;

    int i,count=0,pc=0;
    wchar_t *wc;
    switch(o->format)
    {
    case ASCII:
        for(i=0;i<o->size;i++)
        {
            if(i==0)
            {
                if(o->data[0]=='.' || o->data[0]=='-' || isdigit(o->data[0]))
                    count++;
                if(o->data[0]=='.')
                    pc++;
            }
            else
            {
                if(o->data[i]=='.' || isdigit(o->data[i]))
                    count++;
                if(o->data[i]=='.')
                    pc++;

            }
        }
        break;
    case UTF:
        wc=(wchar_t *)o->data;
        for(i=0;i<o->size;i++)
        {
            if(i==0)
            {
                if(wc[0]=='.' || wc[0]=='-' || isdigit(wc[0]))
                    count++;
                if(wc[0]=='.')
                    pc++;
            }
            else
            {
                if(wc[i]=='.' || isdigit(wc[i]))
                    count++;
                if(wc[i]=='.')
                    pc++;

            }
        }
        break;
    }

    return (o->size==count && pc<2);
}


bool gString::isInteger() const
{
    if(!d)return false;
    ca;

    int i,count=0;
    wchar_t *wc;
    switch(o->format)
    {
    case ASCII:
        for(i=0;i<o->size;i++)
        {
            if(i==0)
            {
                if(o->data[0]=='-' || isdigit(o->data[0]))
                    count++;

            }
            else
            {
                if(isdigit(o->data[i]))
                    count++;


            }
        }
        break;
    case UTF:
        wc=(wchar_t *)o->data;
        for(i=0;i<o->size;i++)
        {
            if(i==0)
            {
                if(wc[0]=='-' || isdigit(wc[0]))
                    count++;
            }
            else
            {
                if(isdigit(wc[i]))
                    count++;

            }
        }
        break;
    }

    return (o->size==count);


}


bool gString::isUInteger() const
{
    if(!d)return false;
    ca;

    int i,count=0;
    wchar_t *wc;
    switch(o->format)
    {
    case ASCII:
        for(i=0;i<o->size;i++)
        {
           if(isdigit(o->data[i]))
                count++;
        }
        break;
    case UTF:
        wc=(wchar_t *)o->data;
        for(i=0;i<o->size;i++)
        {

          if(isdigit(wc[i]))
            count++;
        }
        break;
    }

    return (o->size==count);
}

gString gString::toAscii() const
{
    acn;
    ca;
    gString r;

    r.setString(*this);
    r.setShared(true);
    if(o->format==UTF)
        r.thisToAscii();

    return gString(r);

 }

gString gString::toUnicode() const
{
    acn;
    ca;
    gString r;

    r.setString(*this);
    r.setShared(true);
    if(o->format==ASCII)
        r.thisToUnicode();

    return gString(r);
}

void gString::thisToAscii()
{
    acn;
    ca;
    wchar_t *op;
    char *snew;//new string
    if(o->format==ASCII)return;//nothing to do


    op=(wchar_t *)o->data;

    snew=(char *)malloc(o->size+1);
    snew[o->size]=0;//nullpointer
    wcstombs(snew,op,o->size);

    //free old memory
    free(op);

    //set new pointer
    o->data=snew;
    //set new byte size
    o->sizebytes=o->size;
    o->format=ASCII;
}

//other way around
void gString::thisToUnicode()
{
    acn;
    ca;
    char *op;
    wchar_t *snew;//new string
    if(o->format==UTF)return;//nothing to do


    op=o->data;

    o->sizebytes=o->size * sizeof(wchar_t);
    snew=(wchar_t *)malloc(o->sizebytes + sizeof(wchar_t));
    snew[o->size]=0;//nullpointer
    mbstowcs(snew,op,o->size);

    //free old memory
    free(op);

    //set new pointer
    o->data=(char *)snew;
    o->format=UTF;
}


//comparison

bool gString::operator == (const char *s) const
{
    cn false;
    if(!s)return false;
    ca;
    int ret;
    char *data;
    gString str;
    if(o->format==UTF)
    {

        str=toAscii();
        data=str.ascii();
    }
    else
        data=o->data;
    ret=strcmp(data,s);

    return ret==0;
}

bool gString::operator == (const wchar_t *s) const
{
    cn false;
    if(!s)return false;
    ca;
    int ret;
    wchar_t *data;
    gString str;
    if(o->format==ASCII)
    {

        str=toUnicode();
        data=str.unicode();
    }
    else
        data=(wchar_t *)o->data;
    ret=wcscmp(data,s);

    return ret==0;
}

bool gString::operator == (const gString &other) const
{
    cn false;
    co;
    if(!io)return false;
    ca;

    if(o->format==ASCII)
        return other == o->data;
    else
        return other == (const wchar_t *)o->data;
}
bool gString::operator == (const std::string &stdstring) const{
    if(stdstring.empty())return false;
    cn false;
    return *this == stdstring.c_str();
}
bool gString::operator == (const std::wstring &wstdstring) const{
    if(wstdstring.empty())return false;
    cn false;
    return *this == wstdstring.c_str();
}
bool gString::operator != (const char *s) const
{
    cn false;
    if(!s)return false;
    ca;
    int ret;
    char *data;
    gString str;
    if(o->format==UTF)
    {

        str=toAscii();
        data=str.ascii();
    }
    else
        data=o->data;
    ret=strcmp(data,s);

    return ret!=0;
}

bool gString::operator != (const wchar_t *s) const
{
    cn false;
    if(!s)return false;
    ca;
    int ret;
    wchar_t *data;
    gString str;
    if(o->format==ASCII)
    {

        str=toUnicode();
        data=str.unicode();
    }
    else
        data=(wchar_t *)o->data;
    ret=wcscmp(data,s);

    return ret!=0;
}

bool gString::operator != (const gString &other) const
{
    cn false;
    co;
    if(!io)return false;
    ca;

    if(o->format==ASCII)
        return other != o->data;
    else
        return other != (const wchar_t *)o->data;
}
bool gString::operator != (const std::string &stdstring) const{
    if(stdstring.empty())return false;
    cn false;
    return *this != stdstring.c_str();
}
bool gString::operator != (const std::wstring &wstdstring) const{
    if(wstdstring.empty())return false;
    cn false;
    return *this != wstdstring.c_str();
}

void gString::thisToLowCase()
{
    acn;
    ca;


    char *p;
    wchar_t *wp,*wdata;
    int i;
    if(o->format==ASCII)
    {

        for(i=0;i<o->size;i++)
        {
            p=&o->data[i];
            *p=tolower(*p);
        }
    }
    else
    {
        wp=(wchar_t *)o->data;
        wdata=wp;
        for(i=0;i<o->size;i++)
        {
            wp=&wdata[i];
            *wp=towlower(*wp);

        }
    }

}

void gString::thisToUpperCase()
{
    acn;
    ca;


    char *p;
    wchar_t *wp,*wdata;
    int i;
    if(o->format==ASCII)
    {

        for(i=0;i<o->size;i++)
        {
            p=&o->data[i];
            *p=toupper(*p);

        }
    }
    else
    {
        wp=(wchar_t *)o->data;
        wdata=wp;
        for(i=0;i<o->size;i++)
        {
             wp=&wdata[i];
            *wp=towupper(*wp);

        }
    }

}

gString gString::toLowCase() const
{
    gString r=*this;
    r.thisToLowCase();
    r.setShared(true);
    return gString(r);
}

gString gString::toUpperCase() const
{
    gString r=*this;
    r.thisToUpperCase();
    r.setShared(true);
    return gString(r);
}
std::string gString::toStdString() const{
    cn std::string();//can't do much string is empty
    ca;
    if(o->format==UTF){
        gString s=*this;
        s.thisToAscii();
        return std::string(s.ascii());
    }else{
        return std::string(o->data);
    }
}
std::wstring gString::towStdString() const{
    cn std::wstring();
    ca;
    if(o->format==ASCII){
        gString s=*this;
        s.thisToUnicode();
        return std::wstring(s.unicode());
    }else{
        return std::wstring((wchar_t *)o->data);
    }
}
gString gString::subString(gu32 pos) const
{
    if(!d)return gString();

    ca;
    gassert(pos<o->size,"Buffer overrun gString::subString(pos)");
    if(o->format==ASCII)
    {
        return gString(&o->data[pos],true);
    }
    else
    {
        wchar_t *wc=(wchar_t *)(&o->data[pos]);
        return gString(wc,true);
    }
}


gString gString::subString(gu32 pos, gu32 lenght) const
{
    if(!d)return gString();
    ca;
    gu32 realsize;
    gString r;
    if(lenght > 0)
    {
        realsize=lenght;
        gassert((lenght + pos) <= o->size,"Buffer overrun gString::subString(pos,lenght)");
    }
    else
    {
        gassert(pos < o->size,"Buffer overrun gString::subString(pos,lenght)");
        realsize=o->size - pos;
    }

    r.alloc(realsize,o->format);
    gStringPrivate *io=(gStringPrivate *)r.d;
    if(o->format==ASCII)
    {
        memcpy(io->data,&o->data[pos],realsize);
    }
    else if(o->format==UTF)
    {
        wchar_t *src,*dest;
        src=(wchar_t *)o->data;
        dest=(wchar_t *)io->data;
        memcpy(dest,&src[pos],realsize * sizeof(wchar_t));
    }

    r.setShared(true);
    return gString(r);
}

int gString::strPos(const gString &other,int pos) const{
    ca;
    if(!o)return -1;

    if(pos>=o->size)
        return -1;
    int tpos=pos; //Position
    char *szPointer=NULL;
    char *szIterator; //Iterator pointer
    wchar_t *wszPointer=0;
    wchar_t *wszIterator=0;
    if(o->format==ASCII){
        szPointer=strstr(&o->data[pos],other.ascii());
        szIterator=&o->data[pos];
        if(szPointer){
            while(szPointer!=szIterator){
                szIterator++;
                tpos++;
            }
        }
        else{
            return -1;
        }
    }else{
        wchar_t *t=(wchar_t *)o->data;
        wszPointer=wcsstr(&t[pos],other.unicode());
        wszIterator=&t[pos];
        if(wszPointer){
            while(wszPointer!=wszIterator){
                wszIterator++;
                tpos++;
            }
        }
        else{
            return -1;
        }
    }


       return tpos;
}
//Converts a u16 value to hex and attach it to string
void gString::appendUint16AsHex(gu16 value)
{
    gString ret=gString::uint16toHex(value);

    concatToThis(ret,4);
}
// Gets the hash value of the string
gu32 gString::hash() const{
    if(!d)return 0;
    ca;
    gu32 _hash = 5381;
    if(o->format == ASCII){
        const char *str=o->data;
        for(gu32 i=0;i<o->size;i++){
            _hash=((_hash << 5) + _hash) + (int)str[i];
        }
    }else{
        const wchar_t *str = (wchar_t *)o->data;
        for(gu32 i=0;i<o->size;i++){
            _hash=((_hash << 5) + _hash) + (int)str[i];
        }
    }
    return _hash;

}
//Utility static functions
gString gString::fromByteArray(const gByteArray &ba)
{
    gString s;

    s.alloc(ba.size(),ASCII);

    memcpy(s.ascii(),ba.data(),s.size());

    s.setShared(true);

    return gString(s);
}

gString gString::uint16toHex(gu16 source)
{
    gString s;
    char buff[5];
    buff[0]=0;
    sprintf(buff,"%4x",source);

    for(int i=0;i<4;i++)
    {
        if(buff[i]==' ')
            buff[i]=0;
    }

    s.setString(buff,4);

    s.setShared(true);

    return gString(s);
}

gString gString::uint32toHex(gu32 source)
{
    gString s;
    char buff[9];
    buff[0]=0;
    sprintf(buff,"%8x",source);

    for(int i=0;i<8;i++)
    {
        if(buff[i]==' ')
            buff[i]=0;
    }

    s.setString(buff,8);

    s.setShared(true);

    return gString(s);
}

gu16 gString::fromHexToUint16(const gString &src)
{
    unsigned short ret;
    gassert(!src.isEmpty(),"source string is empty");
    if(src.format()==ASCII)
    {
        sscanf(src.ascii(),"%4x",&ret);
    }
    else
    {
        gString temp=src;

        temp.thisToAscii();
        sscanf(temp.ascii(),"%4x",&ret);
    }

    return ret;

}

gUShortArray gString::fromHexToUint16Array(const gString &src)
{
    gUShortArray arr;
     gassert(!src.isEmpty(),"source string is empty");
    //get the count
    gu32 count=src.size();
    gassert((count % 4)==0,"Source String count must be dividable by 4");

    count=count / 4;

    arr.alloc(count);
    unsigned short *sa,val;
    sa=arr.data();
    //start getting data
    //make a temp buffer
    char *buff;
    gu32 *passer;//trick to skip bytes of 4
    gString tmp;
    if(src.format()==ASCII)
    {
        buff=src.ascii();
    }
    else
    {
        tmp=src;
        tmp.thisToAscii();
        buff=tmp.ascii();
    }
    passer=(gu32 *)buff;
    for(int i=0;i<count;i++)
    {
        sscanf((char *)&passer[i],"%4x",&val);
        sa[i]=val;
    }

    arr.setShared(true);
    return gUShortArray(arr);
}
//This function can be improved
gString gString::htmlEntitiesEncode(const gString &src){
    if(src.isEmpty())return gString();
    gString r;

    if(src.format()==ASCII){
        char *f=src.ascii();

        for(int i=0;i<src.size();i++){
            switch(f[i]){
            case '&':  r+="&amp;";       break;
            case '\"': r+="&quot;";      break;
            case '\'': r+="&apos;";      break;
            case '<':  r+="&lt;";        break;
            case '>':  r+="&gt;";        break;
            default: r+=f[i];
            }
        }
    }
    else{
        wchar_t *f=src.unicode();

        for(int i=0;i<src.size();i++){
            switch(f[i]){
            case '&':  r+=L"&amp;";       break;
            case '\"': r+=L"&quot;";      break;
            case '\'': r+=L"&apos;";      break;
            case '<':  r+=L"&lt;";        break;
            case '>':  r+=L"&gt;";        break;
            default: r+=f[i];
            }
        }

    }

    r.setShared(true);

    return gString(r);

}

gStringList gString::split(const gString &delimiter) const{
    if(isEmpty())
        return gStringList();
    int pos=0;
    int lenght;
    gStringList res;
    gIntArray positions;
    int sizedelimiter=delimiter.size();
    while((pos=strPos(delimiter,pos))!=-1){
        positions.insert(pos);
        pos+=sizedelimiter;
    }
    if(positions.isEmpty()){
        //no delimiter found return the list with a single element containing this string.
       return gStringList(*this,true);
    }
    int *data=positions.data();
    int isize=positions.size() - 1;

    pos=0;
    for(int i=0;i<positions.size();i++){
        if(data[i]==0){
            pos=+sizedelimiter;
            i++;
         }

        lenght=data[i] - pos;
        res.append(subString(pos,lenght));
        pos=data[i]+sizedelimiter;
        if(i==isize)
        {
            //there is a tail missing
            if(data[i]!=(size() - sizedelimiter)){
                lenght=size() - (pos);
                res.append(subString(pos,lenght));
            }
        }


    }

    res.setShared(true);
    return gStringList(res);

}

gString gString::replace(const gString &replaceWhat,const gString &replacement) const{
    acn;
    ca;

    gList<gString> result;
    gString final;
    result=split(replaceWhat);
    gu32 isize=result.size();
    for(int i=0;i<isize;i++){
        final+=result[i];
        if(i< isize - 1){
            final+=replacement;
        }
    }
    if(result.isEmpty()){
        final.setString(*this);
    }
    final.setShared(true);

    return gString(final);
}
//in this version it searches a from the list and replaces with the other list lists must have same sizes
gString gString::replace(const gByteArray &charList, const gByteArray &replacement) const{
    acn;
    ca;
    gString ret;
    ret=*this;
    gassert(!charList.isEmpty() && !replacement.isEmpty(),"LISTS ARE EMPTY");
    gassert((charList.size() == replacement.size()) || replacement.size()==1,"List size mismatch");
    char *clist=(char *)charList.data();
    char *rlist=(char *)replacement.data();
    gu32 repsize=replacement.size();
    if(o->format==ASCII){
        char *data=ret.ascii();
        for(int i=0;i<o->size;i++){
            for(int j=0;j<charList.size();j++){
                if(o->data[i]==clist[j])
                    if(repsize>1)
                        data[i]=rlist[i];
                    else
                        data[i]=rlist[0];
            }
        }
    }else if(o->format==UTF){
        wchar_t *dataret=ret.unicode();
        wchar_t *data=(wchar_t *)o->data;
        for(int i=0;i<o->size;i++){
            for(int j=0;j<replacement.size();j++){
            if(data[i]==(wchar_t)clist[i])
                if(repsize > 1)
                    dataret[i]=(wchar_t)rlist[i];
                else
                    dataret[i]=(wchar_t)rlist[0];
            }

        }
    }

    ret.setShared(true);
    return gString(ret);
}



