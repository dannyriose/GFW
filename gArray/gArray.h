/****************************************gArray Class*********************************/
/*     FILE: gArray.h
       Description: gArray class encapsulates a very generic list class array
       items are stored in sequencial pointer array represented by 32 bit or 64 bit values depending on the
       operating system.
 */
#ifndef GARRAY_H
#define GARRAY_H

#include <gfw_global.h>
#include <gMemory/gBaseArray.h>
#ifdef GUSE_VIRTUALMEMORY
#include <gVirtualMemory/gVirtualMemory.h>
#endif
namespace gfw{
//! Defines a factory for the array element. Use this optional instead of callback functions
struct gArrayElementFactory{
    virtual void *create(const void *) = 0;
    virtual void remove(void *) = 0;
    virtual void deleteme() = 0;
    virtual ~gArrayElementFactory(){

    }
};

    //! Utility class that represents a generic dynamic array of pointer values.
    /** Elements on gArray class are stored as memory pointers in contiguous memory. Therefore, it
      is fast accesing each element of the array using indexes. Notice that only the internal array of pointers which is in reality
      a chunk of 32 bit integers or 64 bit integers (depending on the compiler) is configured as aligned memory.
      When first initialized with gArray::alloc, the internal array is allocated but each value of the array is set to 0
      you can set the value of each array element later filling it with memory pointers.
      If set a cleaning function, the object will automatically call this function when either gArray::clear is called or object
      gets destroyed.
      */
    class SHARED_GFW gArray:public gBaseShared{

    public:
        ///////****/////
        //! Constructor.
        gArray();
        //! Copy constructor
        /** \param other: gArray element where will be construct from. It copies everything, except user pointers.
            See gArray.cpp to see implementation */
        gArray(const gArray &other);//Copy constructor
        //! Intializes array with an initial reference pointer.
        /** \param initialData: Reference pointer to be set as the first element of the array. */
        gArray(const void *initialData);//
        //! Initilizes object given a data block. Check implementation on gArray.cpp
        /** \param DataBlock: Could be an array of any kind but must be of contiguos memory.
            \param dataSize: The size of each element of DataBlock use sizeof(element) to set this value.
            \param uSize: The size or element count contained in DataBlock.
            */
        gArray(const void *DataBlock,gu32 dataSize,gu32 uSize);//With an initial data block of size
        //Destructor
        //! Class destructor
        ~gArray();
        //preallocation
        //! Allocated memory for the array
        /** \param usize: Size of array or total count of array elements.
            \return true if successful, false if failed
            */
        bool alloc(gu32 usize);
        //! Deallocates memory used by the gArray object but only the array leaving the private member intact.
        /** If a clean function has been and bClearUserdata is true then the clean function is called
          per array element.This funciton is ignored if the object does not own the private member.
          \param bClearUserData: Set as true if you want it to call the clean function per array element */
        void clearArray(bool bClearUserData=true);
        //Adding data
        //! Sets the value of an array element given an index.
        /** Refecente pointer data is set as the array element value for the given index.
          \param data: Reference pointer to be stored in the array.
          \param index: Address in array where data reference pointer will be stored.
          \param removeOld: If set true, this function will call the Clean Function passing as parameter
                            the old reference pointer.*/
        void setData(void *data,gu32 index,bool removeOld=true);//sets data given an index
        //Insert at end of array
        //! Inserts at the end of the array a reference pointer.
        /** \param data: Reference poitner to be added to the array. */
        void insertAtEnd(const void *data);
        //! Inserts or appends at the end of the array another array object.
        /** Notice reference pointers are just passed as is. Data is not copied here.
          \param other: gArray object to be added to the end of array. */
        void insertAtEnd(const gArray &other);
        //! Inserts or appends a data block or array at the end of the array.
        /** \param dataBlock: Data or array to be inserted to the array.
            \param blockSize: Size of each array element contained on dataBlock.
            \param arrSize: Size of dataBlock array (not in bytes) or total count of elements. */
        void insertAtEnd(const void *dataBlock,gu32 blockSize,gu32 arrSize);
        //Insert at start of array
        //! Inserts at the first position of the array
        /** \param data: Reference pointer to be added to the array */
        void insertAtFirst(const void *data);
        //! Inserts another gArray object at the begining of the  array
        /** Notice reference pointers are kept as is, data is not copied
          \param other: gArray object to be inserted */
        void insertAtFirst(const gArray &other);
        //! Inserts a databloc or primitive array at the begining of the array.
        /** \param data: Pointer containing the  array to be added.
            \param blockSize: Size of each element contained in data.
            \param arrSize: Total count of elements contained in data. */
        void insertAtFirst(const void *data,gu32 blockSize,gu32 arrSize);
        //Insert at pos
        //! Inserts a reference pointer at given position in the array.
        /** gArray::insert is similar to gArray::insertAtFirst and gArray::insertAtEnd, however, with this function
            you specify the exact position where the item would be inserted.
            \param data: Reference pointer to be added to the array.
            \param pos: 0 based index telling the position where data would be added. */
        void insert(const void *data,gu32 pos);
        //! Inserts all reference pointers contained on another gArray object at a given position in the array.
        /** gArray::insert is similar to gArray::insertAtFirst and gArray::insertAtEnd, however, with this function
            you specify the exact position where the item would be inserted.
            \param other: gArray object to be added to the array.
            \param pos: 0 based index telling the position where data would be added. */
        void insert(const gArray &other,gu32 pos);
        //! Inserts a primitive array at given position in the Garray object.
        /** gArray::insert is similar to gArray::insertAtFirst and gArray::insertAtEnd, however, with this function
            you specify the exact position where the item would be inserted.
            \param data: Data block containing all array elements to be added as reference pointers.
            \param blockSize: Size of array element in bytes.
            \param pos: 0 based index telling the position where data would be added. */
        void insert(const void *data,gu32 blockSize,gu32 arrSize,gu32 pos);
        //Data reading
        //! Gets a reference pointer or array element from the gArray object given an index.
        /** Use this function to access data from the array using 0 based indexes.
          \param index: 0 based index of the array element we want to get.
          \return Reference pointer or array element. Returns 0 if failed.
          This function may return 0 if array is empty or if index is over passes array boundaries */
        void *getData(gu32 index) const;//get data
        //! Const version of gArray::getData
        /** Returns the array element specified by index.
          \param index: 0 based index of the array element we want to get.
          \return Reference pointer or array element. Returns 0 if failed.
          This function may return 0 if array is empty or if index is over passes array boundaries */
        const void *constData(gu32 index) const;
        //! Returns the total count of elements contained in the array.
        /** \return Unsigned integer value contained the total count of elements contained in this array. */
        gu32 size() const; //get the array size
        //locking
        //! Locks the  array only for reading using function gArray::constData
        void lock();//locks the array for writing data can only be read by function constData
        //! Unlocks the array
        void unlock();//unlocks the array
        //removing
        //returns the reference of the removed object if freememory is false other wise returns 0
        //! Removes n quantity of elements from the array starting from position 0
        /** \param blockSize: Quantity of elements to be removed
            \param bFreeMemory: If set true and clean function has been set, then this method calls the cleaning
            function per array element removed from the array.*/
        void *removeAtFirst(gu32 blockSize=1,bool bFreeMemory=false);
        //! Removes n quantity of elements from the array starting from position size -1
        /** \param blockSize: Quantity of elements to be removed
            \param bFreeMemory: If set true and clean function has been set, then this method calls the cleaning
            function per array element removed from the array.*/
        void *removeAtEnd(gu32 blockSize=1,bool bFreeMemory=false);
        //! Removes n quantity of elements from the array starting from a given position
        /** \param index: Position from which the method will begin removing elements from the array.
            \param blockSize: Quantity of elements to be removed
            \param bFreeMemory: If set true and clean function has been set, then this method calls the cleaning
            function per array element removed from the array.*/
        void *removeAt(gu32 index,gu32 blockSize=1,bool bFreeMemory=false);
        //! Removes a block of elements starting from a given memory address.
        /** \param element: Element to remove from array if exists.
         *  \param blockSize: Quantity of elements to be removed starting from where element was found.
         *  \param bFreeMemory: If set true and clean function has been set, then this method calls the cleaning function.*/
        void *removeAt(void *element, gu32 blockSize=1, bool bFreeMemory=false);
        //Misc
        //! Sets the callback cleaning function
        /** Function must have the same structure as typedef IClean
          \param func: Pointer to callback function address */
        void cleanFunction(IClean func);
        //! Sets the callback allocation function
        /** Function must have the same structure as typedef IAlloc
         * \param func: Pointer to callback function address */
        void allocFunction(IAlloc func);
        //! Sets the factory per array element. Use this instead of callback functions.
        /** \param obj: Pointer to object derived from gArrayElementFactory */
        void setElementFactory(gArrayElementFactory *obj);
        //! Returns whether the gArray is empty or not
        /** \return true if gArray is empty or contain 0 elements */
        virtual bool isEmpty() const;
        //! Returns whether the gArray object is locked or not.
        /** \return true if gArray object is locked */
        bool isLocked() const;
        //resize
        //! Expands or shrinks of the gArray object.
        /** If expanded old values are kept intact. If shrinked old values are removed from the list
          \param newSize: New size of the array.
          \param bFreeMemory: If set true and gArray object is being shrinked, this function will call the clean function per
          removed item, but only if the clean function has been set */
        void resize(gu32 newSize,bool bFreeMemory=false);
        //! Makes a binary search of a memory element.
        /** \param element: Pointer to search in array.
         *  \param indexOut: Optional address to variable to retrieve index where element was found.
         * \return true if found false otherwise. */
        bool contains(const void *element,gu32 *indexOut=0) const;
        //! Makes a binary search of a memory element starting from a position.
        /** \param element: Pointer to search in array.
         * \param startPos: Position to start searching element.
         * \param indexOut: Optional address of variable to retrieve index where element was found.
         * \return true if element was found false if not found */
        bool search(const void *element, gu32 startPos=0, gu32 *indexOut=0, G_SEARCH_OPERATOR op=G_SEARCH_EQUAL) const;
        //! Retrieves the total count of elements in the array. Same as size()
        /** \return total count of elements in array */
        gu32 count() const;
        //! Retrieves the total count of matches of elements in the array.
        /** \param element: Element to search in array.
         *  \return total count of matches. */
        gu32 count(const void *element,gUIntArray *outIndexes=0,G_SEARCH_OPERATOR op=G_SEARCH_EQUAL) const;
        //Some useful operators
        //! Returns an array element given an index.
        /** This function works the same as gArray::getData and it is included just for convenience and familiarity of the
          common C++ array structure
          \param index: 0-Based index of the array element.
          \return Array element or 0 if index is out of boundaries or empty. */
        void *operator[](gu32 index) const;
        //! Assign operator.
        /** This operator either passes the private member as reference from other to this gArray object or copies
          the entire content of 'other' gArray to this gArray. This behaviour is determined by gArray::setAsReference or gArray::setByPass
          when 'other' has been set as reference, the operator only copies the address of the private member m_d to this gArray,
          and takes owner ship of it. This means when 'other' gArray object is destroyed or calls clean, the private member will stay
          intact as the new object as taken owner ship of it. This method was implemented to avoid waiting and resources copying data from
          one array to other.
          \param other: gArray object to be assigned.
          \return reference of gArray object in this case it returns *this */
        gArray &operator=(const gArray &other);
        //! Returns the array pointer as array of pointer values.
        /** \return reference pointer of internal data. */
        const guptr *arrayPointer() const;
        //! Returns the pointer of the owner of the private member of the object.
        /** \return pointer of the gArray object owner of the private member m_d. */
        const void *owner() const;
        //! Sets if the array avoid adding duplicate addresses.
        /** \param bSet: Sets the flag if the array should avoid adding duplicates or not*/
        void setAvoidDuplicates(bool bSet);
        //! Gets if the array avoid adding duplicates or not.
        /** \return true if it checks for adding duplicates else not. */
        bool avoidDuplicates() const;
#ifdef GUSE_VIRTUALMEMORY
        //Usage of memory
        //! Sets a gVirtualMemory to use as memory allocator
        /** Although using virtual memory as allocator is not a good idea for production software, it is useful
          to monitor the behaviour of this object. This more useful for debuging purposes.
          \param vmObj: Pointer to gVirtualMemory object */
        void setVirtualMemory(gVirtualMemory *vmObj);
#endif

    };


    //! Macro utility to declare the element factory in a single line
#define G_DECLARE_GARRAY_ELEMENT_FACTORY(name,type) struct name:public gArrayElementFactory{\
                                                        virtual void *create(const void *other) {\
                                                            if(other) \
                                                                return new type(*static_cast<const type *>(other)); \
                                                             else \
                                                                return new type; \
                                                        }\
                                                        virtual remove(void *d) {\
                                                            delete static_cast<type*>(d); \
                                                        }\
                                                        virtual void deleteme(){\
                                                            delete this; \
                                                        } \
                                                        ~name(){} \
                                                    }

}


#endif // GARRAY_H
