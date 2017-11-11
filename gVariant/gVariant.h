#ifndef GVARIANT_H
#define GVARIANT_H
#include <gString/gString.h>
//! Internal macros
#define GVAR_TYPE_CHECK(type) gassert(type==m_type,"gVariant() Type not match")
#define GVARIANT_DIRECT_CAST(type) static_cast<type *>(m_data)
#define GVARIANT_CAST(type) *(static_cast<type *>(m_data))
#define GVARIANT_CAST_OTHER(type) *(static_cast<type *>(other.m_data))
#define GVARIANT_DELETE(type) delete static_cast<type *>(m_data)


namespace gfw{
/**********************************GVariant Types as Strings****************
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
****************************************/
    enum GVARIANT_TYPE{
        GVARIANT_TYPE_CHAR,
        GVARIANT_TYPE_UCHAR,
        GVARIANT_TYPE_SHORT,
        GVARIANT_TYPE_USHORT,
        GVARIANT_TYPE_INT,
        GVARIANT_TYPE_UINT,
        GVARIANT_TYPE_INT64,
        GVARIANT_TYPE_UINT64,
        GVARIANT_TYPE_FLOAT,
        GVARIANT_TYPE_DOUBLE,
        GVARIANT_TYPE_STRING,
        GVARIANT_TYPE_POINTER,
        GVARIANT_TYPE_UNASSIGNED,
        //! Size of this enum
        GVARIANT_TYPE_SIZE
    };

    //! The variant class is a dynamic type object that allows representing most data types.
    class SHARED_GFW gVariant{
    protected:
        //! Data handler on a generic pointer
        gHandle m_data;
        //! Type of pointer it holds
        gs32 m_type;
    public:
        //! Base Constructor
        gVariant():m_data(0),m_type(GVARIANT_TYPE_UNASSIGNED){}
        //! Copy constructor
        gVariant(const gVariant &other);
        //! Constructor takes a char as parameter
        gVariant(gs8 value);
        gVariant(gu8 value);
        gVariant(gs16 value);
        gVariant(gu16 value);
        gVariant(gs32 value);
        gVariant(gu32 value);
        gVariant(gs64 value);
        gVariant(gu64 value);
        gVariant(gf32 value);
        gVariant(gf64 value);
        gVariant(const gString &value);
        gVariant(const char *value);
        gVariant(const wchar_t *value);
        gVariant(void *_data,gs32 _type):m_data(_data),m_type(_type){}
        //! Destructor
        virtual ~gVariant();

        void setValue(gs8 value);
        void setValue(gu8 value);
        void setValue(gs16 value);
        void setValue(gu16 value);
        void setValue(gs32 value);
        void setValue(gu32 value);
        void setValue(gs64 value);
        void setValue(gu64 value);
        void setValue(gf32 value);
        void setValue(gf64 value);
        void setValue(const gString &value);
        void setValue(const char *value);
        void setValue(const wchar_t *value);
        void setPointer(void *_data, gs32 _type);
        bool alloc(GVARIANT_TYPE _type);
        ginline void setType(GVARIANT_TYPE _type){
            m_type=_type;
        }

        //These are fast functions but unsafe make sure you know data type is correct or else it will generate
        //an exception
        ginline gs8 &toChar(){return GVARIANT_CAST(gs8);}
        ginline const gs8 &toChar() const{return GVARIANT_CAST(const gs8);}
        ginline gu8 &toUChar(){return GVARIANT_CAST(gu8);}
        ginline const gu8 &toUChar() const{return GVARIANT_CAST(const gu8);}
        ginline gs16 &toShort(){return GVARIANT_CAST(gs16);}
        ginline const gs16 &toShort() const{return GVARIANT_CAST(const gs16);}
        ginline gu16 &toUShort(){return GVARIANT_CAST(gu16);}
        ginline const gu16 &toUShort() const{return GVARIANT_CAST(const gu16);}
        ginline gs32 &toInt(){return GVARIANT_CAST(gs32);}
        ginline const gs32 &toInt() const{return GVARIANT_CAST(const gs32);}
        ginline gu32 &toUInt(){return GVARIANT_CAST(gu32);}
        ginline const gu32 &toUInt() const{return GVARIANT_CAST(const gu32);}
        ginline gs64 &toInt64(){return GVARIANT_CAST(gs64);}
        ginline const gs64 &toInt64() const{return GVARIANT_CAST(const gs64);}
        ginline gu64 &toUInt64(){return GVARIANT_CAST(gu64);}
        ginline const gu64 &toUInt64() const{return GVARIANT_CAST(const gu64);}
        ginline gf32 &toFloat(){return GVARIANT_CAST(gf32);}
        ginline const gf32 &toFloat() const{return GVARIANT_CAST(const gf32);}
        ginline gf64 &toDouble(){return GVARIANT_CAST(gf64);}
        ginline const gf64 &toDouble() const{return GVARIANT_CAST(const gf64);}
        ginline gString &toString(){return GVARIANT_CAST(gString );}
        ginline const gString &toString() const {return GVARIANT_CAST(const gString);}
        ginline const gScalar &toScalar() const {return GVARIANT_CAST(const gScalar);}
        ginline gScalar &toScalar() {return GVARIANT_CAST(gScalar);}
        ginline operator gs8&(){return GVARIANT_CAST(gs8);}
        ginline operator const gs8&() const{return GVARIANT_CAST(gs8);}
        ginline operator gu8&(){return GVARIANT_CAST(gu8);}
        ginline operator const gu8&() const{return GVARIANT_CAST(gu8);}
        ginline operator gs16&(){return GVARIANT_CAST(gs16);}
        ginline operator const gs16&() const{return GVARIANT_CAST(gs16);}
        ginline operator gu16&(){return GVARIANT_CAST(gu16);}
        ginline operator const gu16&() const{return GVARIANT_CAST(gu16);}
        ginline operator gs32&(){return GVARIANT_CAST(gs32);}
        ginline operator const gs32&() const{return GVARIANT_CAST(gs32);}
        ginline operator gu32&(){return GVARIANT_CAST(gu32);}
        ginline operator const gu32&() const{return GVARIANT_CAST(gu32);}
        ginline operator gs64&(){return GVARIANT_CAST(gs64);}
        ginline operator const gs64&() const{return GVARIANT_CAST(gs64);}
        ginline operator gu64&(){return GVARIANT_CAST(gu64);}
        ginline operator const gu64&() const{return GVARIANT_CAST(gu64);}
        ginline operator gf32&(){return GVARIANT_CAST(gf32);}
        ginline operator const gf32&() const{return GVARIANT_CAST(gf32);}
        ginline operator gf64&(){return GVARIANT_CAST(gf64);}
        ginline operator const gf64&() const{return GVARIANT_CAST(gf64);}
        ginline operator gString&(){return GVARIANT_CAST(gString);}
        ginline operator const gString&() const{return GVARIANT_CAST(gString);}
        //! Gets the value as a string
        gString getAsString() const;
        //! Copies the value from one variant to this variant
        void copy(const gVariant &other);
        //! Clears the structure
        virtual void clear();

        //!
        inline bool isEmpty() const{return m_data == 0;}

        inline const gs32 &type() const{return m_type;}
        inline gHandle data() const{return m_data;}
        inline gHandle data(){return m_data;}

        //Operators
        ginline gVariant &operator = (const gVariant &other){copy(other);return *this;}
        ginline gVariant &operator = (gs8 value){setValue(value);return *this;}
        ginline gVariant &operator = (gu8 value){setValue(value);return *this;}
        ginline gVariant &operator = (gs16 value){setValue(value);return *this;}
        ginline gVariant &operator = (gu16 value){setValue(value);return *this;}
        ginline gVariant &operator = (gs32 value){setValue(value);return *this;}
        ginline gVariant &operator = (gu32 value){setValue(value);return *this;}
        ginline gVariant &operator = (gs64 value){setValue(value);return *this;}
        ginline gVariant &operator = (gu64 value){setValue(value);return *this;}
        ginline gVariant &operator = (gf32 value){setValue(value);return *this;}
        ginline gVariant &operator = (gf64 value){setValue(value);return *this;}
        ginline gVariant &operator = (const gString &value){setValue(value);return *this;}
        ginline gVariant &operator = (const char *value){setValue(value);return *this;}
        ginline gVariant &operator = (const wchar_t *value){setValue(value);return *this;}

        //Arithmetic operators


        ginline gVariant &operator +=(const gVariant &other){opArithObject(other,0);return *this;}
        ginline gVariant &operator -=(const gVariant &other){opArithObject(other,1);return *this;}
        ginline gVariant &operator *=(const gVariant &other){opArithObject(other,2);return *this;}
        ginline gVariant &operator /=(const gVariant &other){opArithObject(other,3);return *this;}


        ginline gVariant operator + (const gVariant &other) const{gVariant v(*this);v+=other;return v;}
        ginline gVariant operator - (const gVariant &other) const{gVariant v(*this);v-=other;return v;}
        ginline gVariant operator * (const gVariant &other) const{gVariant v(*this);v*=other;return v;}
        ginline gVariant operator / (const gVariant &other) const{gVariant v(*this);v/=other;return v;}

        //function versions of arithmetic operators
        ginline void suminc(const gVariant &other){opArithObject(other,0);}
        ginline void subinc(const gVariant &other){opArithObject(other,1);}
        ginline void mulinc(const gVariant &other){opArithObject(other,2);}
        ginline void divinc(const gVariant &other){opArithObject(other,3);}

        ginline gVariant sum(const gVariant &other) const{gVariant v(*this);v+=other;return v;}
        ginline gVariant sub(const gVariant &other) const{gVariant v(*this);v-=other;return v;}
        ginline gVariant mul(const gVariant &other) const{gVariant v(*this);v*=other;return v;}
        ginline gVariant div(const gVariant &other) const{gVariant v(*this);v/=other;return v;}

        ginline bool operator == (const gVariant &other) const {return opBoolObject(other,0);}
        ginline bool operator != (const gVariant &other) const {return opBoolObject(other,1);}
        ginline bool operator < (const gVariant &other) const {return opBoolObject(other,2);}
        ginline bool operator > (const gVariant &other) const {return opBoolObject(other,3);}
        ginline bool operator <= (const gVariant &other) const {return opBoolObject(other,4);}
        ginline bool operator >= (const gVariant &other) const {return opBoolObject(other,5);}
        virtual void setFromString(const gString &value, gs32 _type);
        virtual void setFromString(const gString &value, const gString &_type);
        static gString variantTypeToString(GVARIANT_TYPE _type);
        static GVARIANT_TYPE stringToVariantType(const gString &_type);
        static int variantSize(GVARIANT_TYPE _type);
        static gVariant fromVariantType(GVARIANT_TYPE _type);

    protected:
        //! Template function helper to arithmetic operations
        template <typename T>
        void opArithValue(T value,int op){
            if(!m_data){
                setValue(value);
            }
            switch(m_type){
            case GVARIANT_TYPE_CHAR:
                switch(op){
                case 0:GVARIANT_CAST(gs8)+=(gs8)value;break;
                case 1:GVARIANT_CAST(gs8)-=(gs8)value;break;
                case 2:GVARIANT_CAST(gs8)*=(gs8)value;break;
                case 3:gassert(value!=0,"gVariant() Div by 0");GVARIANT_CAST(gs8)/=(gs8)value;break;
                }
                break;
            case GVARIANT_TYPE_UCHAR:
                switch(op){
                case 0:GVARIANT_CAST(gu8)+=(gu8)value;break;
                case 1:GVARIANT_CAST(gu8)-=(gu8)value;break;
                case 2:GVARIANT_CAST(gu8)*=(gu8)value;break;
                case 3:gassert(value!=0,"gVariant() Div by 0");GVARIANT_CAST(gu8)/=(gu8)value;break;
                }
                break;
            case GVARIANT_TYPE_SHORT:
                switch(op){
                case 0:GVARIANT_CAST(gs16)+=(gs16)value;break;
                case 1:GVARIANT_CAST(gs16)-=(gs16)value;break;
                case 2:GVARIANT_CAST(gs16)*=(gs16)value;break;
                case 3:gassert(value!=0,"gVariant() Div by 0");GVARIANT_CAST(gs16)/=(gs16)value;break;
                }
                break;
            case GVARIANT_TYPE_USHORT:
                switch(op){
                case 0:GVARIANT_CAST(gu16)+=(gu16)value;break;
                case 1:GVARIANT_CAST(gu16)-=(gu16)value;break;
                case 2:GVARIANT_CAST(gu16)*=(gu16)value;break;
                case 3:gassert(value!=0,"gVariant() Div by 0");GVARIANT_CAST(gu16)/=(gu16)value;break;
                }
                break;
            case GVARIANT_TYPE_INT:
                switch(op){
                case 0:GVARIANT_CAST(gs32)+=(gs32)value;break;
                case 1:GVARIANT_CAST(gs32)-=(gs32)value;break;
                case 2:GVARIANT_CAST(gs32)*=(gs32)value;break;
                case 3:gassert(value!=0,"gVariant() Div by 0");GVARIANT_CAST(gs32)/=(gs32)value;break;
                }
                break;
            case GVARIANT_TYPE_UINT:
                switch(op){
                case 0:GVARIANT_CAST(gu32)+=(gu32)value;break;
                case 1:GVARIANT_CAST(gu32)-=(gu32)value;break;
                case 2:GVARIANT_CAST(gu32)*=(gu32)value;break;
                case 3:gassert(value!=0,"gVariant() Div by 0");GVARIANT_CAST(gu32)/=(gu32)value;break;
                }
                break;
            case GVARIANT_TYPE_INT64:
                switch(op){
                case 0:GVARIANT_CAST(gs64)+=(gs64)value;break;
                case 1:GVARIANT_CAST(gs64)-=(gs64)value;break;
                case 2:GVARIANT_CAST(gs64)*=(gs64)value;break;
                case 3:gassert(value!=0,"gVariant() Div by 0");GVARIANT_CAST(gs64)/=(gs64)value;break;
                }
                break;
            case GVARIANT_TYPE_UINT64:
                switch(op){
                case 0:GVARIANT_CAST(gu64)+=(gu64)value;break;
                case 1:GVARIANT_CAST(gu64)-=(gu64)value;break;
                case 2:GVARIANT_CAST(gu64)*=(gu64)value;break;
                case 3:gassert(value!=0,"gVariant() Div by 0");GVARIANT_CAST(gu64)/=(gu64)value;break;
                }
                break;
            case GVARIANT_TYPE_FLOAT:
                switch(op){
                case 0:GVARIANT_CAST(gf32)+=(gf32)value;break;
                case 1:GVARIANT_CAST(gf32)-=(gf32)value;break;
                case 2:GVARIANT_CAST(gf32)*=(gf32)value;break;
                case 3:gassert(value!=0,"gVariant() Div by 0");GVARIANT_CAST(gf32)/=(gf32)value;break;
                }
                break;
            case GVARIANT_TYPE_DOUBLE:
                switch(op){
                case 0:GVARIANT_CAST(gf64)+=(gf64)value;break;
                case 1:GVARIANT_CAST(gf64)-=(gf64)value;break;
                case 2:GVARIANT_CAST(gf64)*=(gf64)value;break;
                case 3:gassert(value!=0,"gVariant() Div by 0");GVARIANT_CAST(gf64)/=(gf64)value;break;
                }
                break;
            }
        }
        //! function helper for arithmetic operations
        void opArithObject(const gVariant &other,int op){
            switch(other.m_type){
            case GVARIANT_TYPE_CHAR:opArithValue<gs8>(GVARIANT_CAST_OTHER(gs8),op);break;
            case GVARIANT_TYPE_UCHAR:opArithValue<gs8>(GVARIANT_CAST_OTHER(gu8),op);break;
            case GVARIANT_TYPE_SHORT:opArithValue<gs16>(GVARIANT_CAST_OTHER(gs16),op);break;
            case GVARIANT_TYPE_USHORT:opArithValue<gu16>(GVARIANT_CAST_OTHER(gu16),op);break;
            case GVARIANT_TYPE_INT:opArithValue<gs32>(GVARIANT_CAST_OTHER(gs32),op);break;
            case GVARIANT_TYPE_UINT:opArithValue<gu32>(GVARIANT_CAST_OTHER(gu32),op);break;
            case GVARIANT_TYPE_INT64:opArithValue<gs64>(GVARIANT_CAST_OTHER(gs64),op);break;
            case GVARIANT_TYPE_UINT64:opArithValue<gu64>(GVARIANT_CAST_OTHER(gu64),op);break;
            case GVARIANT_TYPE_FLOAT:opArithValue<gf32>(GVARIANT_CAST_OTHER(gf32),op);break;
            case GVARIANT_TYPE_DOUBLE:opArithValue<gf64>(GVARIANT_CAST_OTHER(gf64),op);break;
            case GVARIANT_TYPE_STRING:
                if(m_type == GVARIANT_TYPE_STRING){
                    switch(op){
                    case 0:(GVARIANT_CAST(gString))+=(GVARIANT_CAST_OTHER(gString));break;
                    default:
                        break;
                    }
                }
            default:
                break;
            }
        }
        //! Function helper boolean operations
        template <typename T>
        bool opBool(T value, int op) const{
            if(!m_data)return false;

            switch(m_type){
            case GVARIANT_TYPE_CHAR:
                switch(op){
                case 0: return GVARIANT_CAST(gs8) == (gs8)value;
                case 1: return GVARIANT_CAST(gs8) != (gs8)value;
                case 2: return GVARIANT_CAST(gs8) < (gs8)value;
                case 3: return GVARIANT_CAST(gs8) > (gs8)value;
                case 4: return GVARIANT_CAST(gs8) <= (gs8)value;
                case 5: return GVARIANT_CAST(gs8) >= (gs8)value;
                }
                break;
            case GVARIANT_TYPE_UCHAR:
                switch(op){
                case 0: return GVARIANT_CAST(gu8) == (gu8)value;
                case 1: return GVARIANT_CAST(gu8) != (gu8)value;
                case 2: return GVARIANT_CAST(gu8) < (gu8)value;
                case 3: return GVARIANT_CAST(gu8) > (gu8)value;
                case 4: return GVARIANT_CAST(gu8) <= (gu8)value;
                case 5: return GVARIANT_CAST(gu8) >= (gu8)value;
                }
                break;
            case GVARIANT_TYPE_SHORT:
                switch(op){
                case 0: return GVARIANT_CAST(gs16) == (gs16)value;
                case 1: return GVARIANT_CAST(gs16) != (gs16)value;
                case 2: return GVARIANT_CAST(gs16) < (gs16)value;
                case 3: return GVARIANT_CAST(gs16) > (gs16)value;
                case 4: return GVARIANT_CAST(gs16) <= (gs16)value;
                case 5: return GVARIANT_CAST(gs16) >= (gs16)value;
                }
                break;
            case GVARIANT_TYPE_USHORT:
                switch(op){
                case 0: return GVARIANT_CAST(gu16) == (gu16)value;
                case 1: return GVARIANT_CAST(gu16) != (gu16)value;
                case 2: return GVARIANT_CAST(gu16) < (gu16)value;
                case 3: return GVARIANT_CAST(gu16) > (gu16)value;
                case 4: return GVARIANT_CAST(gu16) <= (gu16)value;
                case 5: return GVARIANT_CAST(gu16) >= (gu16)value;
                }
                break;
            case GVARIANT_TYPE_INT:
                switch(op){
                case 0: return GVARIANT_CAST(gs32) == (gs32)value;
                case 1: return GVARIANT_CAST(gs32) != (gs32)value;
                case 2: return GVARIANT_CAST(gs32) < (gs32)value;
                case 3: return GVARIANT_CAST(gs32) > (gs32)value;
                case 4: return GVARIANT_CAST(gs32) <= (gs32)value;
                case 5: return GVARIANT_CAST(gs32) >= (gs32)value;
                }
                break;
            case GVARIANT_TYPE_UINT:
                switch(op){
                case 0: return GVARIANT_CAST(gu32) == (gu32)value;
                case 1: return GVARIANT_CAST(gu32) != (gu32)value;
                case 2: return GVARIANT_CAST(gu32) < (gu32)value;
                case 3: return GVARIANT_CAST(gu32) > (gu32)value;
                case 4: return GVARIANT_CAST(gu32) <= (gu32)value;
                case 5: return GVARIANT_CAST(gu32) >= (gu32)value;
                }
                break;
            case GVARIANT_TYPE_INT64:
                switch(op){
                case 0: return GVARIANT_CAST(gs64) == (gs64)value;
                case 1: return GVARIANT_CAST(gs64) != (gs64)value;
                case 2: return GVARIANT_CAST(gs64) < (gs64)value;
                case 3: return GVARIANT_CAST(gs64) > (gs64)value;
                case 4: return GVARIANT_CAST(gs64) <= (gs64)value;
                case 5: return GVARIANT_CAST(gs64) >= (gs64)value;
                }
                break;
            case GVARIANT_TYPE_UINT64:
                switch(op){
                case 0: return GVARIANT_CAST(gu64) == (gu64)value;
                case 1: return GVARIANT_CAST(gu64) != (gu64)value;
                case 2: return GVARIANT_CAST(gu64) < (gu64)value;
                case 3: return GVARIANT_CAST(gu64) > (gu64)value;
                case 4: return GVARIANT_CAST(gu64) <= (gu64)value;
                case 5: return GVARIANT_CAST(gu64) >= (gu64)value;
                }
                break;
            case GVARIANT_TYPE_FLOAT:
                switch(op){
                case 0: return GVARIANT_CAST(gf32) == (gf32)value;
                case 1: return GVARIANT_CAST(gf32) != (gf32)value;
                case 2: return GVARIANT_CAST(gf32) < (gf32)value;
                case 3: return GVARIANT_CAST(gf32) > (gf32)value;
                case 4: return GVARIANT_CAST(gf32) <= (gf32)value;
                case 5: return GVARIANT_CAST(gf32) >= (gf32)value;
                }
                break;
            case GVARIANT_TYPE_DOUBLE:
                switch(op){
                case 0: return GVARIANT_CAST(gf64) == (gf64)value;
                case 1: return GVARIANT_CAST(gf64) != (gf64)value;
                case 2: return GVARIANT_CAST(gf64) < (gf64)value;
                case 3: return GVARIANT_CAST(gf64) > (gf64)value;
                case 4: return GVARIANT_CAST(gf64) <= (gf64)value;
                case 5: return GVARIANT_CAST(gf64) >= (gf64)value;
                }
                break;
            default:
                break;
            }
            return false;
        }
        bool opBoolObject(const gVariant &other,int op) const{
            if(!other.m_data)return false;

            switch(other.m_type){
            case GVARIANT_TYPE_CHAR:return opBool<gs8>(GVARIANT_CAST_OTHER(gs8),op);
            case GVARIANT_TYPE_UCHAR:return opBool<gs8>(GVARIANT_CAST_OTHER(gu8),op);
            case GVARIANT_TYPE_SHORT:return opBool<gs16>(GVARIANT_CAST_OTHER(gs16),op);
            case GVARIANT_TYPE_USHORT:return opBool<gu16>(GVARIANT_CAST_OTHER(gu16),op);
            case GVARIANT_TYPE_INT:return opBool<gs32>(GVARIANT_CAST_OTHER(gs32),op);
            case GVARIANT_TYPE_UINT:return opBool<gu32>(GVARIANT_CAST_OTHER(gu32),op);
            case GVARIANT_TYPE_INT64:return opBool<gs64>(GVARIANT_CAST_OTHER(gs64),op);
            case GVARIANT_TYPE_UINT64:return opBool<gu64>(GVARIANT_CAST_OTHER(gu64),op);
            case GVARIANT_TYPE_FLOAT:return opBool<gf32>(GVARIANT_CAST_OTHER(gf32),op);
            case GVARIANT_TYPE_DOUBLE:return opBool<gf64>(GVARIANT_CAST_OTHER(gf64),op);
            case GVARIANT_TYPE_POINTER:
                switch(op){
                case 0: return m_data == other.m_data;
                case 1: return m_data != other.m_data;
                default:return false;
                }
                break;
            case GVARIANT_TYPE_STRING:
                if(m_type==GVARIANT_TYPE_STRING){
                    switch(op){
                    case 0: return GVARIANT_CAST(gString) == GVARIANT_CAST_OTHER(gString);
                    case 1: return GVARIANT_CAST(gString) != GVARIANT_CAST_OTHER(gString);
                    }
                }
                break;
            default:
                break;
            }
            return false;
        }



    };

}
#endif // GVARIANT_H
