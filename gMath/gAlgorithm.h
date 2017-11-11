#ifndef GALGORITHM_H
#define GALGORITHM_H
#include <gfw_global.h>

namespace gfw{

#define GSORTFUNCTION_DECLARE(name,type,code) struct name: public gSortFunction<type> \
                                         { \
                                            virtual bool compare(const type &v1,const type &v2) const { \
                                                return code; \
                                            } \
                                         }


template <class T>
struct gSortFunction{
    virtual bool compare(const T &v1, const T &v2) const{
        GFW_UNUSED(v1);
        GFW_UNUSED(v2);
        return false;
    }
    bool operator()(const T &v1, const T &v2) const{
        return compare(v1,v2);
    }
};

struct SHARED_GFW gAlgorithm{



    template <class L, class T>
    ginline static void sort(L &_list,
                             const gSortFunction<T> &_sortFunc ){

        gu32 i = 0;
        gu32 j = 0,topIndex = 0;
        T p1,p2,top;
        bool bDiffValue=true;
        gu32 _listSize = _list.size() - 1;

        while(i < _listSize){
            p1 = _list.value(i);
            p2 = _list.value(j);
            if(j == i) //Check on if counter is 0
            {
                top = p1;
                topIndex = i;
                bDiffValue = false;
            }
            if(_sortFunc(p2,top)){ //Check it meets
                top = p2;
                topIndex = j;
                bDiffValue = true;
            }
            if(j == _listSize){
                if(bDiffValue)
                {
                   p1 = _list.value(i);
                   //Swap values
                   _list.setValue(top,i);
                   _list.setValue(p1,topIndex);

                }
                i++;
                j = i;
            }
            else{
                j++;
            }
        }


    }
};

}


#endif // GALGORITHM_H
