#ifndef GFUNCTION_H
#define GFUNCTION_H
#include "gAlignedLists/gAlignedPtrList.h"
#include "gVariant/gAlignedVariantList.h"
namespace gfw{

//Intended to be some sort of notifing object


/*!
 * \brief Utility encapsulates a virtual function to used for many purposes such as
 * callback programing etc.
 */
class gFunction{
public:
    typedef gVariant(*gFunctionCode)();
    gFunction(){}

    virtual ~gFunction(){}

    virtual void addParam(const gVariant &param){
        m_params.append(param);
    }
    virtual gVariant &param(gu32 index){
        return m_params[index];
    }
    virtual void setParam(const gVariant &value, gu32 index){
        m_params[index] = value;
    }

    virtual void removeParam(gu32 index){
        m_params.remove(index);
    }
    virtual bool hasParam(const gVariant &param){
        return m_params.contains(param);
    }

    virtual gVariant &call(gAlignedVariantList *addparams,gu32 index){
        GFW_UNUSED(addparams);
        GFW_UNUSED(index);

        return m_return;
    }

    virtual gVariant &call(gu32 index){
        GFW_UNUSED(index);
        return m_return;
    }

    virtual gVariant &call(){
        return m_return;
    }
    virtual const gVariant &call() const{
        return call();
    }

    gVariant &operator()(){
        return call();
    }
    const gVariant &operator()() const{
        return call();
    }

    virtual void setFunctionCode(gFunctionCode _code);


protected:
    gAlignedVariantList m_params;
    gFunctionCode m_code;
    gVariant m_return;
};

}


#endif // GFUNCTION_H
