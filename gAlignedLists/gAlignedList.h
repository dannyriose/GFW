#ifndef GALIGNEDLIST_H
#define GALIGNEDLIST_H
#include <gfw_global.h>
#include <cstring>
#include <stdlib.h>
#include <gBase/gAllocator.h>
#include <gMath/gAlgorithm.h>
#include <gBase/gPrivateFactory.h>
#define GALIGNEDLIST_PREALLOC 32
namespace gfw{


template <class T, class A = gAllocator<T> >
//////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief The gAlignedListPrivate struct
///Arrays or aligned lists are objects that handle large amounts of memory. Therefore, sharing their data
/// specially between constructors or assign operatiors can be costly. The gAlignedListPrivate is the objects
/// that represents the shared object of the gAlignedList. It contains all data members usind during list
/// operations.
struct gAlignedListPrivate: public gSharedObject{
    //! The array or list data
    T *g_restrict m_data;
    //! The size or used capacity of the list
    gu32 m_size;
    //! The capacity of the list. This is the actual size of the array in memory.
    gu32 m_capacity;
    //! The allocator
    gAllocator<T> *allocator;
    gConstructor<T> m_constructor;
    //! Standard constructor
    gAlignedListPrivate():
        m_data(0),
        m_size(0),
        m_capacity(0),
        allocator(new A){
        allocator->setUserData(gVariant(gu32(GALIGNEDLIST_PREALLOC)));

    }
    //! Standard destructor
    ~gAlignedListPrivate(){
        clear();
    }
    ///////////////////////////////////////////////////////////////////////////////////////
    /// \brief alloc : Allocates memory for the list. It clears previous data.
    /// \param _size : Size of the array to allocate.
    /// \return true if sucessfully false of not succesful
    ///////////////////////////////////////////////////////////////////////////////////////
    bool alloc(gsize _size){
        clear();
        m_data = allocator->alloc(_size);

        if(m_data){
            m_capacity = _size;
            memset(m_data,0,_size * sizeof(T));
            return true;
        }
        return false;
    }
    //////////////////////////////////////////////////////////////////////////////////////////
    /// \brief resize : Resizes the array or reallocate memory for the list.
    /// \param _newsize : Size in T element values to reallocate.
    /// \return true is successful.
    ///
    bool resize(gsize _newsize){
        if(!m_data){
            return alloc(_newsize);
        }else{
            if(_newsize < m_size){
                T *v;
                for(gu32 i=_newsize;i<m_size;i++){
                    v = &m_data[i];
                    m_constructor.destructor(v);
                }
                m_size = _newsize;
            }
            m_capacity = _newsize;
            m_data = allocator->resize(m_data,_newsize);
            return m_data!=0;
        }
    }
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief clear : Clears or frees the memory used by the array. It also calls the destructor for each element
    /// of the list.
    ///
    void clear(){
        if(!m_data) return;
        T *el;
        for(gu32 i = 0; i<m_size;i++){
            el = &m_data[i];
            if(el){
                m_constructor.destructor(&m_data[i]);
            }
        }
        allocator->dealloc(m_data);
        m_data = 0;
        m_size = 0;
        m_capacity = 0;
    }
    //! Removes an item from list. This function could slower as we have to swap array members with constructors.
    /** \param index: Index on list to be removed */
    void remove(gu32 index){
        if(m_size == 0) return;
        T *val = &m_data[index];
        gu32 msize = m_size - 1;
        //start reallocation of members. This process could be slow.
        T *start = val;
        T *end = &m_data[index + 1];
        for(gu32 i = index; i<msize;i++){
            m_constructor.destructor(start);
            m_constructor.constructor(start,*end);
           start++;
           end++;
        }
        m_size = msize;
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief fastRemove : Removes an item from the list by index. This is a fastest version of remove because it
    /// swaps the last element of the array with the one being deleted saving time.
    /// \param index : Index on list to be removed.
    /// \param bUseAssign : If set true the function uses operator = instead of copy constructor which could be faster
    /// depending on the type of object the array is holding.
    ///
    void fastRemove(gu32 index, bool bUseAssign)
    {
        if(m_size == 0) return; // list is empty nothign to do then.
        m_constructor.destructor(&m_data[index]);
        m_size--;
        if(m_size == index) return; //as we decreased size this means we are deleting the last element from the list
        if(bUseAssign)
        {
            m_data[index] = m_data[m_size];
        }
        else{
            m_constructor.constructor(&m_data[index],m_data[m_size]);
        }
    }
    //////////////////////////////////////////////////////////////////
    /// \brief  copy: Copies the data from another list. All pointers and values are deep copied to the
    /// structure.
    /// \param other: Pointer to the private member of the list to be copied.
    virtual ginline void copy(const gAlignedListPrivate<T,A> *other){
        if(alloc(other->m_size)){
            m_size = m_capacity;
            for(gu32 i = 0; i < m_size;i++){
                const T &val = other->m_data[i];
                m_constructor.constructor(&m_data[i],val);
            }
        }
    }


    bool isConstructed(gu32 index){
        char *val = (char *)&m_data[index];



        for(gu32 i = 0; i < sizeof(T);i++){
            if(val[i] != 0){
                return true;
            }
        }
        return false;
    }


};


template <class T,
          class A = gAllocator<T>,
          class F = gPrivateFactory<gAlignedListPrivate<T,A>> >
////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief The gAlignedList class : Template object container of aligned lists or aligned array. List elements
/// are stored in contiguos memory making it very fast. This is the analogous version of std::vector
///
class gAlignedList: public gObjectSharingModel<gAlignedListPrivate<T,A>,F>{

public:
    ///////////////////////////////////////////////////////////
    /// \brief gAlignedList
    ///Standard constructor. Unlike other objects from the G-Framework. This objects initializes its private
    /// member from the beginning.
    gAlignedList(): gObjectSharingModel<gAlignedListPrivate<T,A>,F>(new F){
        this->d = this->d_factory->_new();

    }
    ////////////////////////////////////////////////////////////////
    /// \brief gAlignedList: Standard copy constructor. If the other object is flagged as shared
    /// the constructor just references to the other object private member and increases the reference count.
    /// \param other: gAlignedList object to be copied.
    ///
    gAlignedList(const gAlignedList<T,A,F> &other):
        gObjectSharingModel<gAlignedListPrivate<T,A>,F>(other,new F){

    }
    /// Standard destructor. Dereferences the private members decreasing the reference count and removing
    /// any reference if no other object have increased the refCount.
    virtual ~gAlignedList(){

    }

    ////////////////////////////////////////
    /// \brief clear
    ///Clears the object removing all memory references if no other have increased its reference count.
    /// This functions creates a new private object to start operating with.
    virtual void clear(){
        this->clearPrivate();
        this->d = this->d_factory->_new();
    }
    //! Roughly it constructs each element of the list.
    ginline virtual void fill(){
        if(!this->d->m_size) return;
        for(gu32 i = 0; i< this->d->m_size;i++){
            this->d->m_constructor.constructor(&this->d->m_data[i]);

        }
    }
    ////////////////////////////////////////////////////////////////////////////////////////
    ///\brief Adds a new element to the end of the list. This is the analoguos of push_back
    /// from other containers. The list capacity is increased when the standard size has reached the total
    /// capacity. The allocator operator() determines the size of the new reallocation size.
    ginline virtual void append(const T &val){
        if(this->d->m_size == this->d->m_capacity)
            this->d->resize(this->d->allocator->userData().toUInt() + this->d->m_capacity);
        this->d->m_constructor.constructor(&this->d->m_data[this->d->m_size],val);
        this->d->m_size++;
    }
    ginline gAlignedList<T,A,F> &operator = (const gAlignedList<T,A,F> &other){
        this->assign(other);
        return *this;
    }
    //////////////////////////////////////////////////////////////////////////////////////
    ///\brief Roughly, it sets a value from the array by constructing the selected element.
    /// this would be the same of m_data[index] = vale. Warning this function does not perform runtime
    /// boundary checks. It is designed to be fast.
    /// \param val: Value to be set to the array.
    /// \param index: Index of array where value is set to be stored on.
    ginline virtual void setValue(const T &val,gu32 index){
        this->d->m_constructor.constructor(&this->d->m_data[index],val);
    }
    /////////////////////////////////////////////////////////////////////////////////////
    ///\brief Allocates memory for the list. Old previous values on the list will be removed.
    /// @note : If this object is shared then this value will be changed on other objects holding this same
    /// shared data.
    /// \param _size: Size of list ot be allocated.
    /// \return true if succesful.
    ginline virtual bool alloc(gu32 _size){
        bool r = this->d->alloc(_size);
        if(r){
            this->d->m_size = _size;
        }
        return r;
    }
    /////////////////////////////////////////////////////////////////////////////////
    ///\brief Return a value from the list pointer by _index. This would be the same as
    /// operator []
    /// \param _index: 0 based index pointing to value on array.
    /// \return reference: To value.
    /// \note This function does not check any array boundaries because is designed to be fast.
    /// in any case is a good practice to check if _index >= gAlingedList::size() before doing anything else
    ginline virtual T &value(gu32 _index){
        return this->d->m_data[_index];
    }
    //! Const version of value. See details of gAlignedList::value
    ginline virtual const T &value(gu32 _index) const{
        return this->d->m_data[_index];
    }
    //! Returns the list as a c style array.
    /// \return pointer to array.
    ginline T *data() {
        return this->d->m_data;
    }
    //! Const version. Returns the list as a c style array.
    /// \return pointer to array.
    ginline const T *data() const{
        return this->d->m_data;
    }
    //! Returns the pointer of a list element.
    /** \param index: 0 based index pointing to list element.
     * \return pointer to list element. */
    ginline T *data(gu32 index){
        return &this->d->m_data[index];
    }
    //! Returns a pointer of a list element. const version.
    ginline const T *data(gu32 index) const{
        return &this->d->m_data[index];
    }
    //! Returns the total count of used elements of the list.
    /// \return 32 bit unsigned integer containing the size of the list.
    ginline const gu32 &size() const{
        return this->d->m_size;
    }
    //! Gets the capacity of this list-
    /// \return 32 bit unsigned integer containing the capacity of the list.
    ginline const gu32 &capacity() const{
        return this->d->m_capacity;
    }
    //! \brief Determines whether the list contains an element with certain value.
    /** \param val: value to be searched on the list.
     * \note this function is meant to be implemented on a derived class since not all
     * data types support the == operator */
    ginline virtual bool contains(const T &val, gu32 *indexout = 0) const{
        GFW_UNUSED(val);
        GFW_UNUSED(indexout);
        return false;
    }
    //! Removes an element from the list by index.
    /** see gAlignedListPrivate::remove for details.
     * \param index : 0 based index value pointing to element to be removed.*/
    ginline virtual void remove(gu32 index){
        this->d->remove(index);
    }
    //! Removes the first element on the list matching a value.
    /** \param val: Element to be searched on list to get removed.
     * \note Function gAlignedList::contains must be implemented so this function could work. */
    ginline virtual void remove(const T &val){
        gu32 index;
        if(contains(val,&index)){
            this->d->remove(index);
        }
    }
    //! Removes an element from the list by index using the fast method.
    /** see gAlignedListPrivate::fastRemove for details.
     * \param index: 0 based index value pointing to element to be removed. */
    ginline void fastRemove(gu32 index){
        this->d->fastRemove(index);
    }
    //! Checks whether the list contains elements or not.
    /// \return true if list is empty.
    ginline virtual bool isEmpty() const{
        return this->d->m_size == 0;
    }


    //! Changes the size of the list without losing previous data.
    /** see gAlignedListPrivate::resize for details.
     * \param _size: New size to be set. Notice this change the capacity of the list
     * either shrinking it or expanding it. If shrunk old values are removed. */
    ginline virtual bool resize(gu32 _size){
        bool  ret = this->d->resize(_size);
        if(ret)
        {
            this->d->m_size = this->d->m_capacity;
        }
        return ret;
    }

    //! Allows using the list a c-style array by 0 based indexes.
    /** \param index: 0 based index pointing to list element.
     * \return reference to list element.
     * \note this operation does not check memory boundaries because is meant to be fast. */
    ginline T &operator [] (int index){
        return this->d->m_data[index];
    }
    //! Allows using the list as a c-style array by 0 based indexes. Const version.
    ginline const T &operator[] (int index) const{
        return this->d->m_data[index];
    }

    A *allocator(){return static_cast<A*>(this->d->allocator);}
    const A *allocator() const {static_cast<A*>(this->d->allocator);}

    gConstructor<T> &constructor() {return this->d->m_constructor;}
    const gConstructor<T> &constructor() const{return this->d->m_constructor;}



    void sort(const gSortFunction<T> &func){
        gAlgorithm::sort<gAlignedList<T>,T>(*this,func);
    }

protected:

};

}
#endif // GALIGNEDLIST_H
