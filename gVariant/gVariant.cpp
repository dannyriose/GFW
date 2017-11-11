#include "gVariant.h"
#include <memory.h>
//Macros to speed up coding.
#define GVARIANT_CREATE(value,type,_dtype) if(!m_data){m_data=new type(value); m_type=_dtype;return; }

#define GVARIANT_SETVALUE(value,type,_dtype) GVARIANT_CREATE(value,type,_dtype);GVARIANT_CAST(type)=(type)value
#define GVARIANT_CAST_OBJ(type) static_cast<type *>(v->data())

//! Variant types as strings
namespace gfw{
static const char *_GVARIANT_STRINGS[]={
"char",
"byte",
"short",
"ushort",
"integer",
"uinteger",
"integer64",
"uinteger64",
"real",
"dreal",
"string",
"pointer"
};
static const int _GVARIANT_DATASIZE[]={
    sizeof(gs8),
    sizeof(gu8),
    sizeof(gs16),
    sizeof(gu16),
    sizeof(gs32),
    sizeof(gu32),
    sizeof(gs64),
    sizeof(gu64),
    sizeof(gf32),
    sizeof(gf64),
    -1,
    -1
};
}

using namespace gfw;



//Helper functions




/*******************************gVariant Functions *************************/
gVariant::gVariant(const gVariant &other):m_data(0),m_type(GVARIANT_TYPE_UNASSIGNED){
    copy(other);
}
gVariant::gVariant(gs8 value):m_data(new gs8(value)),m_type(GVARIANT_TYPE_CHAR){}
gVariant::gVariant(gu8 value):m_data(new gu8(value)),m_type(GVARIANT_TYPE_UCHAR){}
gVariant::gVariant(gs16 value):m_data(new gs16(value)),m_type(GVARIANT_TYPE_SHORT){}
gVariant::gVariant(gu16 value):m_data(new gu16(value)),m_type(GVARIANT_TYPE_USHORT){}
gVariant::gVariant(gs32 value):m_data(new gs32(value)),m_type(GVARIANT_TYPE_INT){}
gVariant::gVariant(gu32 value):m_data(new gu32(value)),m_type(GVARIANT_TYPE_UINT){}
gVariant::gVariant(gs64 value):m_data(new gs64(value)),m_type(GVARIANT_TYPE_INT64){}
gVariant::gVariant(gu64 value):m_data(new gu64(value)),m_type(GVARIANT_TYPE_UINT64){}
gVariant::gVariant(gf32 value):m_data(new gf32(value)),m_type(GVARIANT_TYPE_FLOAT){}
gVariant::gVariant(gf64 value):m_data(new gf64(value)),m_type(GVARIANT_TYPE_DOUBLE){}
gVariant::gVariant(const gString &value):m_data(new gString(value)),m_type(GVARIANT_TYPE_STRING){}
gVariant::gVariant(const char *value):m_data(new gString(value)),m_type(GVARIANT_TYPE_STRING){}
gVariant::gVariant(const wchar_t *value):m_data(new gString(value)),m_type(GVARIANT_TYPE_STRING){}

gVariant::~gVariant(){
    clear();
}

void gVariant::clear(){
    if(!m_data)return;
    switch(m_type){
    case GVARIANT_TYPE_CHAR:GVARIANT_DELETE(gs8);break;
    case GVARIANT_TYPE_UCHAR:GVARIANT_DELETE(gu8);break;
    case GVARIANT_TYPE_SHORT:GVARIANT_DELETE(gs16);break;
    case GVARIANT_TYPE_USHORT:GVARIANT_DELETE(gu16);break;
    case GVARIANT_TYPE_INT:GVARIANT_DELETE(gs32);break;
    case GVARIANT_TYPE_UINT:GVARIANT_DELETE(gu32);break;
    case GVARIANT_TYPE_INT64:GVARIANT_DELETE(gs64);break;
    case GVARIANT_TYPE_UINT64:GVARIANT_DELETE(gu64);break;
    case GVARIANT_TYPE_FLOAT:GVARIANT_DELETE(gf32);break;
    case GVARIANT_TYPE_DOUBLE:GVARIANT_DELETE(gf64);break;
    case GVARIANT_TYPE_STRING:GVARIANT_DELETE(gString);break;
    default:break;
    }
    m_data=0;
    m_type=GVARIANT_TYPE_UNASSIGNED;
}
bool gVariant::alloc(GVARIANT_TYPE _type){
    if(m_type == _type)return false;
    clear();
    switch(_type){
    case GVARIANT_TYPE_CHAR:m_data=new gs8;break;
    case GVARIANT_TYPE_UCHAR:m_data=new gu8;break;
    case GVARIANT_TYPE_SHORT:m_data=new gs16;break;
    case GVARIANT_TYPE_USHORT:m_data=new gu16;break;
    case GVARIANT_TYPE_INT:m_data=new gs32;break;
    case GVARIANT_TYPE_UINT:m_data=new gu32;break;
    case GVARIANT_TYPE_INT64:m_data=new gs64;break;
    case GVARIANT_TYPE_UINT64:m_data=new gu64;break;
    case GVARIANT_TYPE_FLOAT:m_data=new gf32;break;
    case GVARIANT_TYPE_DOUBLE:m_data=new gf64;break;
    case GVARIANT_TYPE_STRING:m_data=new gString;break;
    default:break;
    }
    m_type=_type;
    return m_data!=0;

}

void gVariant::setPointer(void *_data, gs32 _type){
    m_data=_data;
    m_type=_type;
}
void gVariant::setValue(const char *value){
    if(m_type!=GVARIANT_TYPE_STRING){
        clear();m_data=new gString(value);m_type=GVARIANT_TYPE_STRING;
        return;
    }
    GVARIANT_CAST(gString) = value;
}
void gVariant::setValue(const wchar_t *value){
    if(m_type!=GVARIANT_TYPE_STRING){
        clear();m_data=new gString(value);m_type=GVARIANT_TYPE_STRING;
        return;
    }
    GVARIANT_CAST(gString) = value;
}

void gVariant::setValue(const gString &value){
    if(m_type!=GVARIANT_TYPE_STRING){
        clear();m_data=new gString(value);m_type=GVARIANT_TYPE_STRING;
        return;
    }
    GVARIANT_CAST(gString) = value;
}
void gVariant::setValue(gs8 value){GVARIANT_SETVALUE(value,gs8,GVARIANT_TYPE_CHAR);}
void gVariant::setValue(gu8 value){GVARIANT_SETVALUE(value,gu8,GVARIANT_TYPE_UCHAR);}
void gVariant::setValue(gs16 value){GVARIANT_SETVALUE(value,gs16,GVARIANT_TYPE_SHORT);}
void gVariant::setValue(gu16 value){GVARIANT_SETVALUE(value,gu16,GVARIANT_TYPE_USHORT);}
void gVariant::setValue(gs32 value){GVARIANT_SETVALUE(value,gs32,GVARIANT_TYPE_INT);}
void gVariant::setValue(gu32 value){GVARIANT_SETVALUE(value,gu32,GVARIANT_TYPE_UINT);}
void gVariant::setValue(gs64 value){GVARIANT_SETVALUE(value,gs64,GVARIANT_TYPE_INT64);}
void gVariant::setValue(gu64 value){GVARIANT_SETVALUE(value,gu64,GVARIANT_TYPE_UINT64);}
void gVariant::setValue(gf32 value){GVARIANT_SETVALUE(value,gf32,GVARIANT_TYPE_FLOAT);}
void gVariant::setValue(gf64 value){GVARIANT_SETVALUE(value,gf64,GVARIANT_TYPE_DOUBLE);}

gString gVariant::getAsString() const{
    if(!m_data)return gString();
    switch(m_type){
    case GVARIANT_TYPE_CHAR:return gString::number((int)GVARIANT_CAST(gs8));
    case GVARIANT_TYPE_UCHAR:return gString::number((gu32)GVARIANT_CAST(gu8));
    case GVARIANT_TYPE_SHORT:return gString::number((int)GVARIANT_CAST(gs16));
    case GVARIANT_TYPE_USHORT:return gString::number((gu32)GVARIANT_CAST(gs16));
    case GVARIANT_TYPE_INT:return gString::number(GVARIANT_CAST(gs32));
    case GVARIANT_TYPE_UINT:return gString::number(GVARIANT_CAST(gu32));
    case GVARIANT_TYPE_INT64:return gString::number(GVARIANT_CAST(gs64));
    case GVARIANT_TYPE_UINT64:return gString::number(GVARIANT_CAST(gu64));
    case GVARIANT_TYPE_FLOAT:return gString::number(GVARIANT_CAST(gf32));
    case GVARIANT_TYPE_DOUBLE:return gString::number(GVARIANT_CAST(gf64));
    case GVARIANT_TYPE_STRING:{
        gString s=GVARIANT_CAST(gString);
        s.setShared(true);
        return gString(s);
    }
    default:
            break;



    }
    return gString();
}

void gVariant::copy(const gVariant &other){
    if(!other.m_data)return;
    clear();
    switch(other.m_type){
    case GVARIANT_TYPE_CHAR:m_data=new gs8(GVARIANT_CAST_OTHER(gs8));break;
    case GVARIANT_TYPE_UCHAR:m_data=new gu8(GVARIANT_CAST_OTHER(gu8));break;
    case GVARIANT_TYPE_SHORT:m_data=new gs16(GVARIANT_CAST_OTHER(gs16));break;
    case GVARIANT_TYPE_USHORT:m_data=new gu16(GVARIANT_CAST_OTHER(gu16));break;
    case GVARIANT_TYPE_INT:m_data=new gs32(GVARIANT_CAST_OTHER(gs32));break;
    case GVARIANT_TYPE_UINT:m_data=new gu32(GVARIANT_CAST_OTHER(gu32));break;
    case GVARIANT_TYPE_INT64:m_data=new gs64(GVARIANT_CAST_OTHER(gs64));break;
    case GVARIANT_TYPE_UINT64:m_data=new gu64(GVARIANT_CAST_OTHER(gu64));break;
    case GVARIANT_TYPE_FLOAT:m_data=new gf32(GVARIANT_CAST_OTHER(gf32));break;
    case GVARIANT_TYPE_DOUBLE:m_data=new gf64(GVARIANT_CAST_OTHER(gf64));break;
    case GVARIANT_TYPE_STRING:m_data=new gString(GVARIANT_CAST_OTHER(gString));break;
    default:
        m_data=other.m_data;
        break;
    }
    m_type=other.m_type;
}

void gVariant::setFromString(const gString &value, const gString &_type){
    setFromString(value,stringToVariantType(_type));
}
void gVariant::setFromString(const gString &value, gs32 _type){

    clear();
    m_type = _type;
    switch(_type){
    case GVARIANT_TYPE_CHAR:m_data=new gs8((gs8)value.toInt());break;
    case GVARIANT_TYPE_SHORT:m_data=new gs16((gs16)value.toInt());break;
    case GVARIANT_TYPE_INT:m_data=new gs32(value.toInt());break;
    case GVARIANT_TYPE_INT64:m_data=new gs64(value.toInt64());break;
    case GVARIANT_TYPE_UCHAR:m_data=new gu8((gu8)value.toInt());break;
    case GVARIANT_TYPE_USHORT:m_data=new gu16((gu16)value.toInt());break;
    case GVARIANT_TYPE_UINT:m_data=new gu32(value.toUInt());break;
    case GVARIANT_TYPE_UINT64:m_data=new gu64(value.toUInt64());break;
    case GVARIANT_TYPE_FLOAT:m_data=new gf32(value.toFloat());break;
    case GVARIANT_TYPE_DOUBLE:m_data=new gf64(value.toDouble());break;
    case GVARIANT_TYPE_STRING:m_data=new gString(value);break;
    default:break;
    }
}

gString gVariant::variantTypeToString(GVARIANT_TYPE _type){
    if(_type > GVARIANT_TYPE_DOUBLE)return gString();
    return gString(_GVARIANT_STRINGS[_type],true);
}
GVARIANT_TYPE gVariant::stringToVariantType(const gString &_type){
    gs32 i;
    for(i=0;i<GVARIANT_TYPE_UNASSIGNED;i++){
        if(_type == _GVARIANT_STRINGS[i]){
            return (GVARIANT_TYPE)i;
        }
    }
    return GVARIANT_TYPE_UNASSIGNED;
}
int gVariant::variantSize(GVARIANT_TYPE _type){
    if(_type>=GVARIANT_TYPE_UNASSIGNED)return -1;
    return _GVARIANT_DATASIZE[_type];
}

gVariant gVariant::fromVariantType(GVARIANT_TYPE _type){
    switch(_type){
    case GVARIANT_TYPE_CHAR:return gVariant(gs8(0));break;
    case GVARIANT_TYPE_SHORT:return gVariant(gs16(0));break;
    case GVARIANT_TYPE_INT:return gVariant(gs32(0));break;
    case GVARIANT_TYPE_INT64:return gVariant(gs64(0));break;
    case GVARIANT_TYPE_UCHAR:return gVariant(gu8(0));break;
    case GVARIANT_TYPE_USHORT:return gVariant(gu16(0));break;
    case GVARIANT_TYPE_UINT:return gVariant(gu32(0));break;
    case GVARIANT_TYPE_UINT64:return gVariant(gu64(0));break;
    case GVARIANT_TYPE_FLOAT:return gVariant(gf32(0));break;
    case GVARIANT_TYPE_DOUBLE:return gVariant(gf64(0));break;
    case GVARIANT_TYPE_STRING:return gVariant(gString());break;
    case GVARIANT_TYPE_POINTER:return gVariant((void *)0,GVARIANT_TYPE_POINTER);break;
    default:break;
    }
    return gVariant();
}
