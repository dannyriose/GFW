#ifndef GSTRINGLIST_H
#define GSTRINGLIST_H
#include "gString.h"

namespace gfw{
    //! Implements an highly specialized string based array
    /** Reimplements template class gList*/
    class gStringList:public gList<gString>{

    public:
        gStringList(){

        }
        gStringList(const gStringList &other):gList<gString>(other){

        }
        gStringList(const gString &initial, bool bShared):gList<gString>(initial,bShared){

        }

        virtual bool searchValueOperator(const gString &term1, const gString &term2, G_SEARCH_OPERATOR op) const{
            bool ret;
            G_OP_EQ(op,term1,term2,ret);
            return ret;
        }

    };
}
#endif // GSTRINGLIST_H
