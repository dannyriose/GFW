#ifndef GDUALLIST_H
#define GDUALLIST_H
//Template for a dual list
#include "gList.h"
namespace gfw
{
    template <class U,class V>
    class gDualList{
        bool m_deleteu;
        bool m_deletev;
    public:

        struct duallist_data{
            U *d1;
            V *d2;

            duallist_data():d1(0),d2(0){}
            duallist_data(U *_d1,V *_d2):d1(_d1),d2(_d2)
            {

            }
            duallist_data(const U &_d1,const U &_d2){
                d1=new U;
                d2=new V;
                *d1=_d1;
                *d2=_d2;
            }

            ~duallist_data()
            {
                if(d1 && m_deleteu)delete d1;
                if(d2 && m_deletev)delete d2;
            }
        };
        gList<duallist_data> m_list;
        gDualList():m_deleteu(true),m_deletev(true)//constructor
        {

        }
        gDualList(bool clearu, bool clearv):m_deleteu(clearu),m_deletev(clearv){

        }

        gDualList(const gDualList<U,V> &other)
        {
            m_list=other.m_list;
        }

        void append(const U &d1,const V &d2)
        {
            duallist_data *dl=new duallist_data(d1,d2);


            m_list.append(dl);
        }

        void append(const U *d1,const V *d2)
        {
            duallist_data *dl=new duallist_data((U *)d1,(V *)d2);
            m_list.append(dl);
        }

        U &getU(gu32 index)
        {
            return *m_list[index].d1;
        }

        V &getV(gu32 index)
        {
            return *m_list[index].d2;
        }

        const U &getU(gu32 index) const
        {
            return *m_list[index].d1;
        }

        const V &getV(gu32 index) const
        {
            return *m_list[index].d2;
        }

        //Alloc
        void alloc(gu32 _size)
        {
            m_list.alloc(_size);
        }

        void clear()
        {
            m_list.clear();
        }


        //set data
        void setValues(const U *d1,const V *d2,gu32 index)
        {
            m_list.setValue(new duallist_data((U *)d1,(V *)d2),index);
        }

        void setValues(const U &d1,const V &d2,gu32 index)
        {
            m_list.setValue(new duallist_data(d1,d2),index);
        }

        gu32 count() const
        {
            return m_list.size();
        }

        bool isEmpty() const
        {
            return m_list.isEmpty();
        }
    };


}
#endif // GDUALLIST_H
