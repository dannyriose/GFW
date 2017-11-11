#ifndef GPLIST_H
#define GPLIST_H

#include "garray.h"
///This object is deprecated and should not be used.
//Generic template list
namespace gfw
{
    template <class T>
    class gpList
    {

    private:
        gArray d;

        T *a()
        {
            return new T;
        }

    public:
        const gArray &data()const
        {
            return d;
        }
        void setRemoveReferences(bool bSet)
        {
            if(bSet)
                d.cleanFunction(gpList::clist);
            else
                d.cleanFunction(NULL);
        }

    //Constructors
        gpList(bool removeReferences=true)
        {
            if(removeReferences)
                d.cleanFunction(gpList::clist);

        }

        gpList(const gpList<T> &other,bool removeReferences=true)
        {
            d.insertAtEnd(other.data());
             if(removeReferences)
                d.cleanFunction(gpList::clist);

        }
        gpList(const T &val,bool removeReferences=true)
        {
            T *n=a();
            *n=val;
            d.insertAtEnd(n);
             if(removeReferences)
                d.cleanFunction(gpList::clist);

        }


      //Data insertion
        void insert(const T &val,gu32 pos)
        {
            T *n=a();
            *n=val;
            d.insert(n,pos);
        }

       void insert(const gpList<T> &other, gu32 pos)
        {
            d.insert(other.data(),pos);
        }

        //Direct references reference pointer must have been created with the
       void insert(const T *ref,gu32 pos)
       {
           d.insert(ref,pos);
       }

    //Append
        void append(const T &val)
        {
            T *n=a();
            *n=val;
            d.insertAtEnd(n);
        }

        void append(const gpList<T> &other)
        {
            d.insertAtEnd(other.data());
        }

        void append(const T *ref)
         {
            d.insertAtEnd(ref);
        }

    //Data removal
        void remove(gu32 pos,gu32 blockSize)
        {
            d.removeAt(pos,blockSize,true);
        }
    //Operators
        gpList<T> &operator=(const gpList<T> &other)
        {
            d=other.data();
        }

      /*  T operator[](gu32 index)const
        {
            T *v;
            v=(T *)d[index];
            if(v)
            {
                return T(*v);
            }
            else
                return T();
        }*/

        T *operator[](gu32 index)
        {
            T *v;
            v=(T *)d[index];
            if(!v && !d.isEmpty() && index<d.size())
            {
                v=a();
                d.setData(v,index);
            }

            return v;

        }

        T *operator[](gu32 index) const
        {
            T *v;
            v=(T *)d[index];
            return v;
        }

        T value(gu32 index) const
        {
            T *v;
            v=(T *)d[index];
            if(v)
            {
                return T(*v);
            }
            else
                return T();
        }




        gu32 size()
        {
            return d.size();
        }

        bool isEmpty()
        {
            return d.isEmpty();
        }

        bool isLocked()
        {
            return d.isLocked();
        }

        void lock(){d.lock();}
        void unlock(){d.unlock();}

        void setValue(const T &value,gu32 index)
        {
            if(index>=d.size())return;
            T *n=(T *)d.getData(index);
            if(!n)
            {
                n=a();
                *n=value;
                d.setData(n,index);
            }
            else
            {
                *n=value;
            }
        }
        void setValue(const T *ref,gu32 index)
        {
            if(index>=d.size())return;
            d.setData(ref,index);
        }

        void alloc(gu32 isize)
        {
            d.alloc(isize);
        }
        void resize(gu32 newSize,bool fm=false)
        {
            d.resize(newSize,fm);
        }

        void clear()
        {
            d.clear();
        }

        static void clist(void *data)
        {
            T *n=(T *)data;

            if(n)
                delete n;
        }


    };
}
#endif // GPLIST_H
