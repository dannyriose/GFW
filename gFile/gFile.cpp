#include "gFilePrivate.h"
#ifdef WIN32
#include <ShlObj.h>
#endif
using namespace gfw;
#define ca gFilePrivate *o=(gFilePrivate *)d
#define co gFilePrivate *io=(gFilePrivate *)other.d
#define acn gassert(d,"File not opened")
gFile::gFile():gBaseShared(new gFilePrivateFactory){
}
gFile::gFile(const gFile &other):gBaseShared(other,new gFilePrivateFactory){

}

gFile::~gFile(){
}

void gFile::close()
{
    if(!d)return;
    ca;
    fclose(o->f);
    clear();
}

bool gFile::open(const gString &file, GFILE_ACCESSMODES amode, GFILE_OPENMODES omode)
{
    gFilePrivate *o;
    FILE *f;
    gString smode;
    gu64 _size=0;
    if(d)
    {
        close();
    }
    if(file.isEmpty())
        return false;
    switch(amode)
    {
    case GFILEAM_ACCESSREAD:
        smode="r";
        break;
    case GFILEAM_ACCESSWRITE:
        smode="w";
        break;
    }
    switch(omode)
    {
    case GFILEOM_BINARY:
        smode+="b";
        break;
    case GFILEOM_TEXT:
        smode+="t";
        break;
    }



    if(file.format()==gString::ASCII)
    {
        if(amode==GFILEAM_ACCESSREAD)
        {
            struct stat fs;
            stat(file.ascii(),&fs);
            _size=fs.st_size;
        }
        f=fopen(file.ascii(),smode.ascii());
    }
    else if(file.format()==gString::UTF)
    {
#ifdef WIN32
        if(amode==GFILEAM_ACCESSREAD)
        {
            struct _stat fs;
            _wstat(file.unicode(),&fs);
            _size=fs.st_size;
        }
        smode.thisToUnicode();
        f=_wfopen(file.unicode(),smode.unicode());
#elif __gnu_linux__
        gString s=file;
        s.thisToAscii();
        if(amode==GFILEAM_ACCESSREAD)
        {
            struct stat fs;
            stat(file.ascii(),&fs);
            _size=fs.st_size;
        }
        f=fopen(s.ascii(),smode.ascii());
#endif
    }
    if(!f)return false;

    o=new gFilePrivate(f,_size,file);

    d=o;
    return true;

}

gsize gFile::read(void *_dest, gsize blockSize, gsize count)
{
    acn;
    ca;

    return fread(_dest,blockSize,count,o->f);
}

gsize gFile::write(const void *src, gsize blockSize, gsize _count)
{
    acn;
    ca;
    return fwrite(src,blockSize,_count,o->f);
}

gByteArray gFile::readAll()
{
    acn;
    ca;
    gByteArray ba;
    ba.alloc(o->size);

    fread(ba.data(),sizeof(char),o->size,o->f);

    ba.setShared(true);
    return gByteArray(ba);

}

gString gFile::readAllToString(gString::GSTRING_FORMAT format, gString::GSTRING_FORMAT fileFormat)
{
    acn;
    ca;
    gString s;
    gsize _datasize;
    void *p;
    s.alloc(o->size,format);

    //be careful to makesure
    switch(fileFormat){
    case gString::ASCII:p=s.ascii();_datasize=sizeof(char);break;
    case gString::UTF:p=s.unicode();_datasize=sizeof(wchar_t);break;
    }


    fread(p,_datasize,o->size,o->f);
    if(format!=fileFormat)
    {
        switch(format){
        case gString::ASCII: s.thisToAscii(); break;
        case gString::UTF: s.thisToUnicode(); break;
        }
    }

    s.setShared(true);
    return gString(s);

}


gsize gFile::write(const gMemory &src)
{
    acn;
    ca;
    gu32 s,bs;


    s=src.size();//size of array
    bs=src.blocksize();//size of a block
    fwrite(&s,sizeof(gu32),1,o->f);
    fwrite(&bs,sizeof(gu32),1,o->f);


    return fwrite(src.data(),sizeof(char),src.sizeInBytes(),o->f);
}

gsize gFile::read(gMemory &out)
{
    acn;
    ca;
    gu32 s,bs;

    fread(&s,sizeof(gu32),1,o->f);
    fread(&bs,sizeof(gu32),1,o->f);

    out.alloc(bs,s);

    return fread(out.data(),sizeof(char),out.sizeInBytes(),o->f);
}

gsize gFile::write(const gString &src, bool bWriteHeaders)
{
    acn;
    ca;
    gu32 s,format;
    s=src.size();
    format=src.format();

    if(bWriteHeaders)
    {
        fwrite(&s,sizeof(gu32),1,o->f);
        if(s==0)return 0;
        fwrite(&format,sizeof(gu32),1,o->f);
    }
    else{
        if(!s)
            return 0;
    }
    if(format==gString::ASCII)
        return fwrite(src.ascii(),sizeof(char),s,o->f);
    else if(format==gString::UTF)
        return fwrite(src.unicode(),sizeof(wchar_t),s,o->f);

    return 0;
}

gsize gFile::read(gString &out, bool hasHeaders,
                  gString::GSTRING_FORMAT formatout,
                  gString::GSTRING_FORMAT fileformat)
{
    acn;
    ca;
    gu32 s,format;

    if(hasHeaders)
    {
        fread(&s,sizeof(gu32),1,o->f);
        if(s==0)return 0;
        fread(&format,sizeof(gu32),1,o->f);
        out.alloc(s,format);

        if(format==gString::ASCII)
            return fread(out.ascii(),sizeof(char),s,o->f);
        else if(format==gString::UTF)
            return fread(out.unicode(),sizeof(wchar_t),s,o->f);
        return 0;
    }
    else
    {
        out = readAllToString(formatout,fileformat);
        return out.size();
    }

}

gsize gFile::seek(gsize offset, GFILE_SEEKORIGINS origin)
{
    acn;
    ca;
    return fseek(o->f,offset,origin);

}

bool gFile::eof() const
{
    acn;
    ca;
    //not yet impletmented for text mode
    return ftell(o->f)>=(long)o->size;
}


bool gFile::isOpened() const
{
    return d!=0;
}

gsize gFile::position() const
{
    acn;
    ca;
    return ftell(o->f);
}

gFile &gFile::operator =(const gFile &other)
{
    assign(other);

    return *this;
}

FILE *gFile::handle() const
{
    acn;
    ca;
    return o->f;
}

gsize gFile::size() const
{
    if(!d)return 0;
    ca;
    if(o->size!=0)
        return o->size;
    else
    {
        gsize currPos=ftell(o->f);
        fseek(o->f,0,SEEK_END);
        o->size=ftell(o->f);
        //get back to cursor
        fseek(o->f,currPos,SEEK_SET);
        return o->size;

    }
}

const gString &gFile::fileName() const
{
    acn;
    ca;
    return (const gString &)o->fname;
}

gsize gFile::write(const gVariant &src)
{
    acn;
    gs32 _type=src.type();
    //first write the type, then the value and if this is a string use the save string func
    gs32 _size;

    if(_type >= GVARIANT_TYPE_SIZE)
        return 0;


    writeInt(_type);

    switch(_type){
    case GVARIANT_TYPE_STRING: return write(src.toString(),true);
    default:
        _size=gVariant::variantSize((GVARIANT_TYPE)_type);
    }
    if(_size==-1)return 0;


    return write(src.data(),(gsize)_size,1);

}


gsize gFile::read(gVariant &out)
{
    //this got to be interesting
    acn;
    gs32 type;
    gsize _size;
    type=readInt();
    out.alloc((GVARIANT_TYPE)type);
    switch(type)
    {
    case GVARIANT_TYPE_STRING:return read(out.toString(),true);
    default:
        _size=gVariant::variantSize((GVARIANT_TYPE)type);
    }
    return read(out.data(),_size,1);
}


gsize gFile::writeInt(gs32 val)
{
    acn;
    ca;
    return fwrite(&val,sizeof(gs32),1,o->f);
}

gs32 gFile::readInt()
{
    acn;
    ca;
    gs32 val;
    fread(&val,sizeof(gs32),1,o->f);
    return val;
}

gsize gFile::writeUInt(gu32 val)
{
    acn;
    ca;
    return fwrite(&val,sizeof(gu32),1,o->f);
}


gu32 gFile::readUInt()
{
    acn;
    ca;
    gu32 val;
    fread(&val,sizeof(gu32),1,o->f);
    return val;
}

gsize gFile::writeScalar(gScalar val){
    acn;
    ca;
    return fwrite(&val,sizeof(gScalar),1,o->f);
}
gScalar gFile::readScalar(){
    acn;
    ca;
    gScalar val;
    fread(&val,sizeof(gScalar),1,o->f);
    return val;
}

//******************************************/
gString gFile::appendExt(const gString &src, const gString &ext)
{
    gString r,ss,localExt;

    gu32 extPos;

    extPos=src.size()-ext.size();

    //get the substring at position extPos

    localExt=ext.toLowCase();
    ss=src.subString(extPos);
    ss.thisToLowCase();
    if(ss==localExt)
    {
        r=src;
        r.setShared(true);
        return gString(r);
    }
    else
    {
        r=src;
        r+=ext;
        r.setShared(true);
        return gString(r);
    }


}

bool gFile::fileExists(const gString &file){
    if(file.isEmpty())return false;
    //may need a better implementation
    if(file.format()==gString::ASCII)
#ifdef WIN32
        return _access(file.ascii(),0)!=-1;
#elif __gnu_linux__
        return access(file.ascii(),0)!=-1;
#endif
    else
#ifdef WIN32
        return _waccess(file.unicode(),0)!=-1;
#elif __gnu_linux__
    {
        gString s=file;
        s.thisToAscii();
        return access(s.ascii(),0)!=-1;
    }
#endif
}

bool gFile::createDirectory(const gString &path){
    if(path.isEmpty())return false;
    if(path.format() == gString::ASCII){
#ifdef WIN32
#ifdef UNICODE
      gString s=path.toUnicode();
       return CreateDirectory(path.unicode(),0);
#else
       return CreateDirectory(path.ascii(),0);
#endif
#elif __gnu_linux__
        return mkdir(path.ascii(),S_IRWXU | S_IRWXG) == 0;
#endif
    }else{
        gString s=path.toAscii();
#ifdef WIN32
#ifdef UNICODE
       return CreateDirectory(path.unicode(),0);
#else
       return CreateDirectory(s.ascii(),0);
#endif
#elif __gnu_linux__
        return mkdir(s.ascii(),S_IRWXU | S_IRWXG) == 0;
#endif
    }
}

//to get the application folder directory
gString gFile::getAppDataDirectory(const gString &appname,bool bCreateDirectory){
    gString dir;
#ifdef __gnu_linux__
 struct passwd *pw = getpwuid(getuid());
 dir=pw->pw_dir;
 dir+=".";
 dir+=appname;

#endif
#ifdef _WIN32
 TCHAR szPath[MAX_PATH];
 // Get path for each computer, non-user specific and non-roaming data.
 if(SUCCEEDED( SHGetFolderPath(NULL,CSIDL_APPDATA,NULL,0,szPath))){
     dir=gString(szPath) + appname;
 }
#endif
 if(bCreateDirectory){
     if(!gFile::fileExists(dir)){
        gFile::createDirectory(dir);
     }
 }
 dir.setShared(true);
 return dir;

}
