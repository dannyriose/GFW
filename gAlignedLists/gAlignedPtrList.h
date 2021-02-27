#ifndef GALIGNEDPTRLIST_H
#define GALIGNEDPTRLIST_H
#include <gfw_global.h>
#include <stdlib.h>
#include <cstring>
#include <gString/gString.h>
#include "gBase/gAllocator.h"
#include "gMath/gAlgorithm.h"
#define GALIGNEDPTRLIST_PREALLOC 32
namespace gfw{

template <class T>
/// \struct gAlignedPtrListNDAllocator
/// \brief Non destructive allocator->
///
struct gAlignedPtrListNDAllocator: public gAllocator<T>{
    ginline virtual void remove(T *ptr){
        GFW_UNUSED(ptr);
    }
};


template <class T , class A = gAllocator<T>>
/*!
 * \brief Shared object of the aligned list. Contains all data, and information about the list
 */
struct gAlignedPtrListPrivate: public gSharedObject{
    /// Main data. An array of pointers of type T
    T **g_restrict m_data;
    /// The actual size of the list
    gu32 m_size;
    /// The capacity of the list
    gu32 m_totalsize;
    /// The Allocator
    gAllocator<T> *allocator;

    gAlignedPtrListPrivate():
        m_data(0),
        m_size(0),
        m_totalsize(0),
        allocator(new A){
        allocator->setUserData(gVariant(gu32(GALIGNEDPTRLIST_PREALLOC)));

    }
    ~gAlignedPtrListPrivate(){
        clear();
        delete allocator;
    }
    bool alloc(gsize _size){
        clear();
        m_data = allocator->template alloc<T*,guptr>(_size);

        if(m_data){
            m_totalsize = _size;
            memset(m_data,0,sizeof(T*) * _size);
            return true;
        }
        return false;
    }
    bool _realloc(gsize _size){
        if(!m_data){
            return alloc(_size);
        }else{

            if(_size < m_size){
                T *val;
                for(gu32 i = _size; i< m_size;i++){
                    val = m_data[i];
                    if(val)
                    {
                        allocator->remove(val);
                    }
                }
                m_size = _size;
            }
            m_totalsize=_size;
            m_data = allocator->template resize<T*,guptr>(m_data,m_totalsize);

        }
        return m_data!=0;
    }
    ginline void prealloc(){
       if(m_totalsize == m_size){
           _realloc(m_totalsize + allocator->userData().toUInt());
        }
    }

    void clear(){
        if(!m_data) return;
        T *el;
        for(gu32 i = 0; i<m_size;i++){
            el =m_data[i];
            if(el){
                allocator->remove(el);
            }
        }
        free(m_data);
        m_data = 0;
        m_size = 0;
        m_totalsize = 0;
    }
    //! Removes a single element from the list. This process is faster than on gAlignedList because
    //! we only swap pointers without having the need to call destructors and constructors.
    ginline bool remove(gu32 index){
        if(m_size == 0)
            return false;
        T *val;
        gu32 msize = m_size - 1;
        val = m_data[index];

        for(gu32 i = index,j = index + 1; i < msize;i++,j++){
            m_data[i] = m_data[j];
        }
        // call the destructor for the removed item
        if(val){
            allocator->remove(val);
        }
        m_size = msize;
        return true;
    }
    //! Faster version of remove. It only swaps the last element with current avoiding the need
    //! to move all other list elements.
    ginline bool fastRemove(gu32 index)
    {
        // list is empty then do nothing
        if(m_size == 0)
            return false;
        //get the element to be removed
        T *val = m_data[index];
        //decrease the size of the list by 1
        m_size--;
        if(val)
        {
            //destruct the element if not null
            allocator->remove(val);
        }
        //if it is removing the last element of the list then do nothing
        if(index == m_size)
        {
           return true;
        }
        //swap the actual last element of the list with
        //the element position being removed
        T *last = m_data[m_size];

        m_data[index] = last;

        return true;

    }
    ginline void copy(const gSharedObject *obj){

        const gAlignedPtrListPrivate<T,A> *other = static_cast<const gAlignedPtrListPrivate<T,A> *>(obj);
        if(alloc(other->m_size)){
            T *val;
            gu32 i;
            m_size = m_totalsize;
            for(i = 0; i < m_size; i++){
                val =other->m_data[i];
                if(val){
                    m_data[i] = allocator->create(*val);
                }
            }
        }

    }

    ginline void fill(){
        if(!m_data) return;
        T *val;

        for(gu32 i = 0; i > m_size; i++){
            val = m_data[i];

            if(!val){
                val = allocator->create();
                m_data[i] = val;
            }
        }
    }



};

//=========================================gAlignedPtrList Implementation====================================
//! Template class that represents a list of points of type T.
template <class T,
          class A = gAllocator<T>,
          class F = gPrivateFactory<gAlignedPtrListPrivate<T,A>>>
/*!
 * \brief Basic list container template. Convinient container of
 */
class gAlignedPtrList: public gObjectSharingModel<gAlignedPtrListPrivate<T,A>,F>{

public:
    gAlignedPtrList():
        gObjectSharingModel<gAlignedPtrListPrivate<T,A>,F>(new F){
        this->d = this->d_factory->_new();

    }
    gAlignedPtrList(const gAlignedPtrList<T,A,F> &other):
        gObjectSharingModel<gAlignedPtrListPrivate<T,A>,F>(other,new F){

    }

    virtual ~gAlignedPtrList(){

    }
    gAlignedPtrList &operator = (const gAlignedPtrList<T,A,F> &other){
        this->assign(other);
        return *this;
    }

    virtual void clear(){
        this->clearPrivate();
        this->d = this->d_factory->_new();
    }
    ginline virtual void append(){
        T *val = this->d->allocator->create();
        append(val);
    }

    //! Adds a pointer to the list. Take into account the list could take ownership of it. All depends upon the allocator->
    ginline virtual void append(T *ref, bool _copy = false){
       T *val;
       this->d->prealloc();
       if(_copy)
       {
           val = this->d->allocator->create(*ref);
       }
       else
       {
           val = ref;
       }
       this->d->m_data[this->d->m_size] = val;
       this->d->m_size++;
    }
    ginline virtual void append(const T &val){
        T *ref = this->d->allocator->create(val);
        append(ref);
    }
    ginline virtual void setValue(T *ref, gu32 index){
        T *oldvalue =this->d-> m_data[index];
        if(oldvalue)
        {
            this->d->allocator->remove(oldvalue);
        }
        this->d->m_data[index] = ref;
    }
    ginline virtual void setValue(const T &val, gu32 index){
        T *_val =  this->d->allocator->create(val);
        setValue(_val,index);
    }

    ginline bool alloc(gu32 _size){
        bool bret = this->d->alloc(_size);
        if(bret){
            this->d->m_size = _size;
        }
        return bret;
    }
    ginline virtual T *value(gu32 _index) const{
        return this->d->m_data[_index];
    }
    ginline T **data(){
        return this->d->m_data;
    }
    ginline const T **data() const{
        return this->d->m_data;
    }

    ginline virtual T *search(const gString &id, gu32 *indexOut = 0) const{
        GFW_UNUSED(id);
        GFW_UNUSED(indexOut);
        return 0;
    }
    ginline virtual T *search(gs32 id, gu32 *indexOut = 0) const{
        GFW_UNUSED(id);
        GFW_UNUSED(indexOut);
        return 0;
    }
    ginline virtual bool contains(T *ref, gu32 *indexout = 0) const{
        for(gu32 i = 0; i < this->d->m_size; i++){
            if(ref == this->d->m_data[i]){
                if(indexout) *indexout = i;
                return true;
            }
        }
        return false;
    }
    ginline virtual bool contains(const gString &id,gu32 *indexOut = 0) const{
       return search(id,indexOut) != 0;
    }
    ginline virtual bool contains(gs32 id,gu32 *indexOut = 0) const{
       return search(id,indexOut) != 0;
    }
    ginline virtual bool remove(gu32 index){
        return this->d->remove(index);
    }
    ginline virtual bool remove(T *ref){
        gu32 index;
        if(contains(ref,&index)){
            return this->d->remove(index);
        }
        return false;
    }
    ginline virtual bool remove(const gString &id){
        gu32 index;
        if(contains(id,&index)){
            return this->d->remove(index);
        }
        return false;
    }
    ginline virtual bool fastRemove(gu32 index){
        return this->d->fastRemove(index);
    }
    ginline virtual bool fastRemove(T *ref){
        gu32 index;
        if(contains(ref,&index)){
            return this->d->fastRemove(index);
        }
        return false;
    }
    ginline virtual bool fastRemove(const gString &id){
        gu32 index;
        if(contains(id,&index)){
            return this->d->fastRemove(index);
        }
        return false;
    }

    ginline const gu32 &size() const{
        return this->d->m_size;
    }
    ginline const gu32 &capacity() const{
        return this->d->m_totalsize;
    }
    ginline virtual bool isEmpty() const{
        return this->d->m_size == 0;
    }
    ginline virtual void copyFromOther(const gAlignedPtrList<T,A,F> &other){
        *this = other;
    }

    ginline virtual bool resize(gu32 _size){
        return this->d->_realloc(_size);
    }

    //not the safest but to comply with some code
    ginline T &operator [] (int index){
        return *this->d->m_data[index];
    }
    ginline const T &operator[] (int index) const{
        return *this->d->m_data[index];
    }

    ginline bool setDestroyData(bool bSet){
        this->d->allocator->setDeleteData(bSet);
        return true;
    }

    ginline bool destroyData() const{return this->d->allocator->deleteData();}
    ginline A *allocator(){return static_cast<A*>(this->d->allocator);}
    ginline const A *allocator() const{return static_cast<A*>(this->d->allocator);}

    ginline virtual void fill(){
        this->d->fill();

    }
    ginline virtual void sort(const gSortFunction<T*> &func){
        bool dp = destroyData();
        if(dp)setDestroyData(false);
        {
            gAlgorithm::sort<gAlignedPtrList<T,A,F>,T*>(*this,func);
        }
        if(dp) setDestroyData(true);
    }



};
}
#endif // GALIGNEDPTRLIST_H
