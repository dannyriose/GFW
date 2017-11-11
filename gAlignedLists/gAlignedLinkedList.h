#ifndef GALIGNEDLINKEDLIST_H
#define GALIGNEDLINKEDLIST_H
#include "gAlignedList.h"

namespace gfw{


template <class T, class A>
struct gAlignedLinkedListNode{
    T *m_data;
    A *allocator;
    gAlignedLinkedListNode<T> *m_prev;
    gAlignedLinkedListNode<T> *m_next;

   gAlignedLinkedListNode():m_data(0),
       m_prev(0),
       m_next(0),
       m_bEnabled(true){

   }
   ~gAlignedLinkedListNode(){
       allocator->destruct(m_data);
   }

public:
   bool m_bEnabled;

};

template <class T>
struct gAlignedLinkedListAllocator{
    gAlignedList<T> m_data;

    virtual gAlignedLinkedListNode<T,gAlignedLinkedListAllocator<T> > *alloc() const{
        m_data.append(gAlignedLinkedListNode<T>());
        gAlignedLinkedListNode<T ,gAlignedLinkedListAllocator<T> > *d = m_data(m_data.size() - 1);
        d->allocator = this;
        return d;
    }
    virtual void dealloc(gAlignedLinkedListNode<T,gAlignedLinkedListAllocator<T> > *node) const{
        node->m_bEnabled = false;
    }
    virtual T *construct() const{
        return new T;
    }
    virtual void destruct(T *ptr) const{
        delete ptr;
    }
    virtual void clear(){
        m_data.clear();
    }
};

template <class T, class A = gAlignedLinkedListAllocator<T> >
struct gAlignedLinkedListPrivate{
    gAlignedLinkedListNode<T,A> *m_first;
    gAlignedLinkedListNode<T,A> *m_last;
    gAlignedLinkedListNode<T,A> *m_node;
    A allocator;

    gAlignedLinkedListPrivate(): m_first(0),
        m_last(0),
        m_node(0){
        m_first = allocator.alloc();
        m_last = m_first;
    }
    ~gAlignedLinkedListPrivate(){

    }
    ginline void clear(){
        allocator.clear();
    }
    ginline void append(T *val){
        m_node = allocator.alloc();
        if(!m_first) m_first = m_node;
        m_node->m_data = val;
        m_node->m_prev = m_last;
        m_node->m_next = 0;
        m_last = m_node;
    }
};
}
#endif // GALIGNEDLINKEDLIST_H
