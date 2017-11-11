#ifndef GALIGNEDVARIANTLIST_H
#define GALIGNEDVARIANTLIST_H

#include <gAlignedLists/gAlignedList.h>
#include "gVariant.h"


#define GVARIANTLIST_CAST(param) *static_cast<param*>(value(index).data())
namespace gfw{

class gAlignedVariantList: public gAlignedList<gVariant>{
public:
    gAlignedVariantList(){

    }

    gAlignedVariantList(const gAlignedVariantList &other):gAlignedList<gVariant>(other){

    }

    ~gAlignedVariantList(){

    }


    ginline gs8 &toChar(gu32 index) {return GVARIANTLIST_CAST(gs8);}
    ginline const gs8 &toChar(gu32 index) const {return GVARIANTLIST_CAST(gs8);}
    ginline gu8 &toUChar(gu32 index) {return GVARIANTLIST_CAST(gu8);}
    ginline const gu8 &toUChar(gu32 index) const {return GVARIANTLIST_CAST(gu8);}
    ginline gs16 &toShort(gu32 index) {return GVARIANTLIST_CAST(gs16);}
    ginline const gs16 &toShort(gu32 index) const {return GVARIANTLIST_CAST(gs16);}
    ginline gu16 &toUShort(gu32 index) {return GVARIANTLIST_CAST(gu16);}
    ginline const gu16 &toUShort(gu32 index) const {return GVARIANTLIST_CAST(gu16);}
    ginline gs32 &toInt(gu32 index) {return GVARIANTLIST_CAST(gs32);}
    ginline const gs32 &toInt(gu32 index) const {return GVARIANTLIST_CAST(gs32);}
    ginline gu32 &toUInt(gu32 index) {return GVARIANTLIST_CAST(gu32);}
    ginline const gu32 &toUInt(gu32 index) const {return GVARIANTLIST_CAST(gu32);}
    ginline gs64 &toInt64(gu32 index) {return GVARIANTLIST_CAST(gs64);}
    ginline const gs64 &toInt64(gu32 index) const {return GVARIANTLIST_CAST(gs64);}
    ginline gu64 &toUInt64(gu32 index) {return GVARIANTLIST_CAST(gu64);}
    ginline const gu64 &toUInt64(gu32 index) const {return GVARIANTLIST_CAST(gu64);}
    ginline gf32 &toFloat(gu32 index) {return GVARIANTLIST_CAST(gf32);}
    ginline const gf32 &toFloat(gu32 index) const {return GVARIANTLIST_CAST(gf32);}
    ginline gf64 &toDouble(gu32 index) {return GVARIANTLIST_CAST(gf64);}
    ginline const gf64 &toDouble(gu32 index) const {return GVARIANTLIST_CAST(gf64);}
    ginline gString &toString(gu32 index) {return GVARIANTLIST_CAST(gString);}
    ginline const gString &toString(gu32 index) const {return GVARIANTLIST_CAST(gString);}
    ginline const gScalar &toScalar(gu32 index) const {return GVARIANTLIST_CAST(const gScalar);}
    ginline gScalar &toScalar(gu32 index) {return GVARIANTLIST_CAST(gScalar);}
    ginline const gs32 &type(gu32 index) const {return this->value(index).type();}


    virtual bool contains(const gVariant &val, gu32 *indexout = 0) const{
        for(gu32 i = 0; i < size(); i++){
            if((d->m_data[i])==val){
                if(indexout){
                    *indexout = i;
                }
                return true;
            }
        }
        return false;
    }

};

}



#endif // GALIGNEDVARIANTLIST_H
