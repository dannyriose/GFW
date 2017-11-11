#ifndef GCONSTRUCTOR_H
#define GCONSTRUCTOR_H


namespace gfw{

template <class T>
class gConstructor{
public:
    virtual void constructor(T *ptr){
        new(ptr) T();
    }

    virtual void constructor(T *ptr, const T &other){
        new(ptr) T(other);
    }

    virtual void destructor(T *ptr){
        ptr->~T();
    }

};


}

#endif // GCONSTRUCTOR_H
