#ifndef GALIGNEDPTRTABLE_H
#define GALIGNEDPTRTABLE_H

#include "gAlignedLists/gAlignedPtrList.h"

namespace gfw{


template <class T,class A>
class gAlignedPtrTablePrivate:public gSharedObject{
public:
    gAlignedPtrTablePrivate():
        m_rows(0),
        m_cols(0){

    }

    gAlignedPtrTablePrivate(const gAlignedPtrTablePrivate<T,A> &other):
        gSharedObject(&other){

    }


    virtual void copy(const gSharedObject *other){
        const gAlignedPtrTablePrivate<T,A> *p = static_cast<gAlignedPtrTablePrivate *>(other);

        m_data = p->m_data;
        m_rows = p->m_rows;
        m_cols = p->m_cols;
    }

public:
    gAlignedPtrList<gAlignedPtrList<T,A>> m_data;
    gu32 m_rows;
    gu32 m_cols;
};

template <class T,
          class A,
          class F = gPrivateFactory<gAlignedPtrTablePrivate<T,A>>>
class gAlignedPtrTable: public gObjectSharingModel<gAlignedPtrTablePrivate<T,A>>{
public:
    gAlignedPtrTable():
        gObjectSharingModel<gAlignedPtrTablePrivate<T,A>>(new F){

    }
    gAlignedPtrTable(const gAlignedPtrTable<T,A> &other):
        gObjectSharingModel<gAlignedPtrTablePrivate<T,A>>(other,new F){

    }
    ~gAlignedPtrTable(){

    }

    virtual void create(gu32 _cols, gu32 _rows){
        this->d->m_data.alloc(_cols);
        gAlignedPtrList<T,A> *list;
        T *val;
        for(gu32 i = 0; i < _cols; i++){
            list = new gAlignedPtrList<T,A>();
            this->d->m_data.setValue(list,i);

            list->alloc(_rows);
            for(gu32 j = 0; j<_rows;j++){
               val = list->allocator().create();
               list->setValue(val,j);
            }

        }
        this->d->m_rows = _rows;
        this->d->m_cols = _cols;
    }

    virtual void appendColumn(){
        gAlignedPtrList<T,A> *col = new gAlignedPtrList<T,A>();

        this->d->m_data.append(col);
        this->d->m_cols++;
    }
    virtual void appendRow(){
        T *val;
        gAlignedPtrList<T,A> *list;
        for(gu32 i = 0; i< this->d->m_data.size(); i++){
            list = this->d->m_data[i];
            val = list->allocator().create();
            list->append(val);
        }
        this->d->m_rows++;
    }
    virtual void appendColumn(const gAlignedPtrList<T,A> &col){
        gassert(col->size() != this->d->m_rows,"gAlignedPtrTable row mismatch size");
        this->d->m_data.append(col);
        this->d->m_cols = this->d->m_data.size();
        this->d->m_cols++;
    }
    virtual void appendRow(const gAlignedPtrList<T,A> &row){
        gu32 i,j;
        gAlignedPtrList<T,A> *list;

        gassert(row.size() == this->d->m_data.size(),"size mismatch gAlignedPtrTable::appendRow()");
        for(i = 0; i < this->d->m_data.size(); i++){
            list = this->d->m_data[i];

            list->append(row[i],true);
        }
        this->d->m_rows++;
    }
    virtual void setValue(const T &_value, gu32 x, gu32 y){
        gAlignedPtrList<T,A> *col = this->d->m_data[x];

        col->setValue(_value,y);
    }
    virtual void setValue(T *_value, gu32 x, gu32 y){
        gAlignedPtrList<T,A> *col = this->d->m_data[x];

        col->setValue(_value,y);
    }

    gAlignedPtrTable<T,A,F> &operator = (const gAlignedPtrTable<T,A,F> &other){
        assign(other);
        return *this;
    }
    virtual T *value(gu32 x,gu32 y){
        gAlignedPtrList<T,A> *col = this->d->m_data[x];
        return col->value(y);
    }
    virtual const T *value(gu32 x,gu32 y) const{
        gAlignedPtrList<T,A> *col = this->d->m_data[x];
        return col->value(y);
    }
    virtual void clear(){
        this->clearPrivate();
        this->d = this->d_factory->create();
    }
    virtual bool contains(T *ref, gu32 *indexOut = 0) const{
       T *val;
       for(gu32 i=0; i< this->d->m_cols;i++){
           for(gu32 j = 0; j<this->d->m_rows;j++){
              val = value(i,j);
              if(val == ref){
                  return true;
              }
           }
       }
       return false;
    }

    virtual T *search(const gString &_id, gu32 *indexOut = 0) const{
        GFW_UNUSED(_id);
        GFW_UNUSED(indexOut);
        return 0;
    }


};

}


#endif // GALIGNEDPTRTABLE_H
