#ifndef GSHAREDPOINTER_H
#define GSHAREDPOINTER_H

#include "gBase/gBaseShared.h"
#include "gBase/gAllocator.h"

namespace gfw{
template <class T,
          class A = gAllocator<T>>
class gSharedPointerPrivate: public gSharedObject{
public:
    gSharedPointerPrivate():
        m_pointer(0),
        allocator(new A){
        this->shareMode = GSHARED_OBJECT_ALWAYS_SHARED;

    }
    gSharedPointerPrivate(const gSharedPointerPrivate<T> *other):
        gSharedObject(other),
        m_pointer(0),
        allocator(new A){
        this->shareMode = GSHARED_OBJECT_ALWAYS_SHARED;

    }
    ~gSharedPointerPrivate(){
        clear();
        delete allocator;
    }

    bool create(){
        //allocator here should have
        clear();
        m_pointer = allocator->create();

    }
    bool create(const T &other){
        clear();
        m_pointer = allocator->create(other);
    }
    void clear(){

            allocator->remove(m_pointer);

    }

    virtual void copy(const gSharedObject *other){
        const gSharedPointerPrivate<T,A> *obj = static_cast<const gSharedPointerPrivate<T,A>*>(other);

        clear();

        m_pointer = new T(*obj->m_pointer);
    }


    T *m_pointer;
    gAllocator<T> *allocator;
};


template <class T,
          class A = gAllocator<T>,
          class F = gPrivateFactory<gSharedPointerPrivate<T,A>>>
class gSharedPointer: public gObjectSharingModel<gSharedPointerPrivate<T,A>,F>{
public:
    gSharedPointer(): gObjectSharingModel<gSharedPointerPrivate<T,A>,F>(new F){
        this->d = this->d_factory->_new();
        this->d->create();

    }
    gSharedPointer(const gSharedPointer<T,A,F> &other):
        gObjectSharingModel<gSharedPointerPrivate<T,A>,F>(other,new F){
        this->d->create();

    }
    gSharedPointer(const T& other):
        gObjectSharingModel<gSharedPointerPrivate<T,A>,F>(other,new F)
    {
        this->d = this->d_factory->_new();
        this->d->create(other);

    }

    gSharedPointer(T *ref):
        gObjectSharingModel<gSharedPointerPrivate<T,A>,F>(new F)
    {
        this->d = this->d_factory->_new();
        this->d->m_pointer = ref;


    }

    ~gSharedPointer(){

    }
    virtual void clear(){
        this->clearPrivate();
        this->d = this->d_factory->_new();
    }

    gSharedPointer &operator = (const gSharedPointer &other){
        assign(other);
        return *this;
    }

    T &operator[](int index){
        return this->d->m_pointer[index];
    }
    const T &operator[](int index) const{
        return this->d->m_pointer[index];
    }
    operator T*() {
        return this->d->m_pointer;
    }

    operator const T*() const{
        return this->d->m_pointer;
    }

    T *pointer() const{
        return this->d->m_pointer;
    }

    void setPointer(T *ref){
        this->d->clear();
        this->d->m_pointer = ref;

    }

};



}


#endif // GSHAREDPOINTER_H
