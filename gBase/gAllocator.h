#ifndef GALLOCATOR_H
#define GALLOCATOR_H
#include "gVariant/gVariant.h"
#include "gConstructor.h"
namespace gfw{
template <class T>
///////
/// \brief The gAlignedListAllocator struct
/// Default allocator for list items.
class gAllocator{
public:
    gAllocator():m_deleteData(false){

    }
    virtual ~gAllocator(){

    }
    ginline virtual T *create(){
        T *ptr = new T();
        return ptr;
    }
    ginline virtual T *create(const T &other){
        return new T(other);
    }

    ginline virtual void remove(T *ptr){
        if(m_deleteData){
            delete ptr;
        }
    }
    //! Allocates memory for the array on the gAlignedList.
    /** \param _size: Size in list element of new array size.
     * \return new allocated memory. */
    template <class V = T, class S = T>
    ginline V *alloc(gu32 _size){
        return (V *)malloc(_size * sizeof(S));
    }

    //! Deallocates or frees the memory of the array.
    /** \param ptr: Pointer to array to be freed.*/
    template <typename V = T>
    ginline void dealloc(V *ptr) const{
        free(ptr);
    }
    //! Resizes or reallocs  memory of the array.
    /** \param ptr: Pointer to array.
     * \param _size: Size in list element for the new size. If this value is higher than the old one
     * then old values will be retain. Else the list will be shrinked.
     * \return pointer to new resized array.*/
    template <typename V = T,
              typename S = T>
    ginline V *resize(V *ptr, gu32 _size){
        return (V *)realloc(ptr,_size * sizeof(S));
    }
    //! Operator returns the default preallocation value for the gAlignedList.
    /// \return preallocation value.
    ginline gVariant &operator()(){
        return m_userdata;
    }
    ginline void setUserData(const gVariant &_userData){
        m_userdata = _userData;
    }
    ginline gVariant &userData(){
        return m_userdata;
    }

    template <typename U = T,
              typename S = T>
    static ginline U *allocate(gu32 _size){
        return static_cast<U*>(malloc(_size * sizeof(S)));
    }
    template <typename U = T,
              typename S = T>
    static ginline void deallocate(U *ptr){
        free(ptr);
    }
    bool deleteData() const{return m_deleteData;}
    void setDeleteData(bool bSet) {m_deleteData = bSet;}

protected:
    bool m_deleteData;
    gVariant m_userdata;
};

}
#endif // GALLOCATOR_H
