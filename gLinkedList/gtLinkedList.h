#ifndef GTLINKEDLIST_H
#define GTLINKEDLIST_H

#include "gLinkedList.h"
#include <gArray/gList.h>
namespace gfw
{
    template <class T>
    class gtLinkedList
    {
    protected:
        gLinkedList d;
    private:
        T *c(){return new T;}
    public:
        const gLinkedList &l() const
        {
            return d;
        }
        gtLinkedList()
        {
            d.create();
            d.setFunction(cleanf);
        }
        gtLinkedList(const gtLinkedList<T> &other)
        {
            bool bp=other.d.isByPass();
            if(bp)
            {
                d=gLinkedList(other.d);
                d.setByPass(true);
            }
            else
            {
                d.append(other.d);
                d.setFunction(cleanf);
            }
        }

        void append(const T &value)
        {
            T *n=c();
            *n=value;
            d.append(n);
        }
        void append(const T *element)
        {
            d.append(element);
        }

        void append(const gtLinkedList<T> &other)
        {
            d.append(other.d);
        }

        void first() const
        {
            d.first();
        }

        void next() const
        {
            d.next();
        }
        void last() const
        {
            d.last();
        }
        T *pvalue() const
        {
            return (T *)d.data();
        }

        T *pvalue(gu32 index) const
        {
            return (T *)d.data(index);
        }

        T &value()
        {
            return *(T *)d.data();
        }
        T &value(gu32 index)
        {
            return *(T *)d.data(index);
        }

        const T &value() const
        {
            T *n=d.data();

            return (const T&)*n;
        }

        const T &value(gu32 index) const
        {
            T *n=d.data(index);
            return (const T &)*n;
        }

        void insert(gu32 index, const T &value)
        {
            T *n=c();
            *n=value;
            d.insert(index,n);
        }
        void insert(gu32 index,const T *value)
        {
            d.insert(index,value);
        }

        void remove()
        {
            d.remove();
        }

        void remove(gu32 index)
        {
            d.remove(index);
        }

        gu32 count() const
        {
            return d.count();
        }

        bool isEmpty() const
        {
            return d.isEmpty();
        }

        void setByPass(bool bSet)
        {
            d.setByPass(bSet);
        }

        static void cleanf(void *data)
        {
            T *n=(T *)data;

            if(n)
                delete n;
        }

        gList<T> toList() const
        {
            if(d.isEmpty())
                return gList<T>();
            T *n;
            gList<T> list;
            gu32 count=0;
            d.first();
            list.alloc(d.count());
            while((n=(T *)d.data())!=0)
            {
                list.setValue(n,count);
                count++;
                d.next();
            }

            list.setRemoveReferences(false);
            return gList<T>(list,false);
        }
        void clear(){
            d.clear();
        }


    };

}
#endif // GTLINKEDLIST_H
