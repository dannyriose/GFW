#ifndef GALIGNEDTABLE_H
#define GALIGNEDTABLE_H
#include <gAlignedLists/gAlignedList.h>

namespace gfw{

template <class T, class A >
class gAlignedTablePrivate: public gSharedObject{
public:
    gAlignedTablePrivate():
        m_columns(0),
        m_rows(0),
        m_size(0){

    }

    ~gAlignedTablePrivate(){

    }

    virtual void copy(const gSharedObject *other){
        gAlignedTablePrivate<T,A> *p = static_cast<gAlignedTablePrivate<T,A>*>(other);
        m_columns = p->m_columns;
        m_rows = p->m_rows;
        m_size= p->m_size;
        m_data = p->m_data;
    }

public:
    gAlignedList<T,A> m_data;  
    gu32 m_columns;
    gu32 m_rows;
    gu32 m_size;
    gConstructor<T> m_cons;
};


template <class T,
          class A =gAllocator<T>,
          class F = gPrivateFactory<gAlignedTablePrivate<T,A>>>
class gAlignedTable: public gObjectSharingModel<gAlignedTablePrivate<T,A>>{
public:
    gAlignedTable():
        gObjectSharingModel<gAlignedTablePrivate<T,A>>(new gPrivateFactory<gAlignedTablePrivate<T,A>>){


    }
    gAlignedTable(const gAlignedTable &other):
         gObjectSharingModel<gAlignedTablePrivate<T,A>>(other,new gPrivateFactory<gAlignedTablePrivate<T,A>>)
    {

    }

    virtual ~gAlignedTable(){

    }

   virtual void create(gu32 _cols,gu32 _rows){
        this->d->m_data.alloc(_cols);

        for(gu32 i=0;i<_cols;i++){
            gAlignedList<T> &_col = this->d->m_data[i];
            //could call fill better call constructor
            this->d->m_data.constructor().constructor(&_col);
            _col.alloc(_rows);


        }
        this->d->m_columns = _cols;
        this->d->m_rows = _rows;
    }

    virtual void clear(){
        this->clearPrivate();
        this->d = this->d_factory->create();
    }

    virtual void setValue(const T &val, gu32 x, gu32 y){
        gAlignedList<T> &col = this->d->m_data[x];
        col.setValue(val,y);
    }

    virtual T &value(gu32 x, gu32 y){
        gAlignedList<T> &col = this->d->m_data[x];
        return col.value(y);
    }
    const virtual T &value(gu32 x, gu32 y) const{
        const gAlignedList<T> &col = this->d->m_data[x];
        return col.value(y);
    }

    virtual gAlignedList<T> &column(gu32 index){
        return this->d->m_data[index];
    }
    virtual const gAlignedList<T> &column(gu32 index) const{
        return this->d->m_data[index];
    }

    gu32 columns() const{
        return this->d->m_columns;
    }
    gu32 rows() const{
        return this->d->m_rows;
    }

    gu32 itemCount() const{
        return this->d->m_columns * this->d->m_rows;
    }




};

}

#endif // GALIGNEDTABLE_H
