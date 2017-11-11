/************************************template class gList***********************************************/
/*     FILE:gList.h
       Description: gList class represents a generic template array list.
       */
#ifndef GLIST_H
#define GLIST_H

#include "gArray.h"
#include <gLinkedList/gLinkedList.h>
#ifdef GLIST_CHECK_DUPLICITY
#define GLIST_DUPLICITY true
#else
#define GLIST_DUPLICITY false
#endif
#ifdef GLIST_DUPLICITY_CHECK_ENABLED
#define dupc(c) if(dupCheck(c)) return
#else
#define dupc(c) (void)0
#endif
namespace gfw
{
    //Generic template list
    //! Utility object to handle dynamic lists of objects.
    /** This class is implemented as a template class to easily handle lists of any.
     It is basically a wrapper of the class gArray that helps to make much easier handling lists.
     Includes most of the functionality of class gArray including clean up.
     The class gList is one of the core objects of the G-Framework as it is used by many other classes
     of the same framework. */
    template <class T>
    class gList
    {

    protected:
        //! Protected member gArray class is where all the list is stored
        gArray d;
        //! When set to true checks for duplicity values.
        bool m_duplicity;
    private:
        //! Private membar just an allocator
        /** Make sure when using this template your object T has a default constructor
           \return a new pointer to a T object type */
        T *a()
        {
            return new T;
        }

    public:
        //! Gets the reference of object gArray
        /** \return reference of gArray object */
        ginline const gArray &data()const { return d;}
        ginline gArray &data() {return d;}
        //! Set the flag is the object would call the 'clean function' when calling clear or on destruction.
        /** \param bSet: Set it true if you want the function use the clean function to remove references of all objects
          added to the list. Set it false otherwise */
        void setRemoveReferences(bool bSet)
        {
            //This function basically sets the clean function or deactivate it
            if(bSet)
                d.cleanFunction(gList::clist);
            else
                d.cleanFunction(0);
        }
        //! Allows setting your own construct function per list itme.
        void setConstructFunction(IAlloc afnc){
            d.allocFunction(afnc);
        }

        //! Allows setting your own clean up function per list item.
        /** Use this function if your objects require something an special clean up routing.
          \param cfnc :Address of custom clean up function */
        void setCleanupFunction(IClean cfnc)
        {
            d.cleanFunction(cfnc);
        }
        //! Initializes the list passing a custom clean up function.
        /** \param cfnc: Address of custom clean up function */
        gList(IClean cfnc):m_duplicity(GLIST_DUPLICITY)
        {
            d.cleanFunction(cfnc);
        }

    //Constructors
        //! Default constructor.
        gList():m_duplicity(GLIST_DUPLICITY){
            d.cleanFunction(gList::clist);
            d.allocFunction(gList::alist);
        }
        //! Construction with option to remove references or not.
        /** \param removeReferences: Set it true if you want the list to perform clean up */
        gList(bool removeReferences):m_duplicity(GLIST_DUPLICITY)
        {
            d.allocFunction(gList::alist);
            if(removeReferences)
                d.cleanFunction(gList::clist);

        }
        //! Copy constructor.
        /** \param other: gList object source to initialize object */
        gList(const gList<T> &other):d(other.d),m_duplicity(other.m_duplicity){
            // actually this has no effect at all if the object is shared or not
            //data is set to be copied internally
            /*if(!d.isShared()) //this is redundant
                d.cleanFunction(gList::clist);*/
        }
        //! Copy constructor with option to remove references.
        /** \param other: gList object source to initialize object.
            \param removeReferences: Set it true to automatically remove references at destruction  */
        gList(const gList<T> &other,bool removeReferences):d(other.d),m_duplicity(other.m_duplicity)
        {

             if(removeReferences)
                d.cleanFunction(gList::clist);

        }
        //! Initializes list with a startup value.
        /** Val is copied not referenced.
          \param val: Value added as initial value.
          \param removeRefernces: Set it true to automatically remove references at destruction */

        gList(const T &val,bool shared = false):m_duplicity(GLIST_DUPLICITY)
        {
            d.allocFunction(gList::alist);
            T *n=a();//allocate memory for the new object
            *n=val;//copy new object
            d.insertAtEnd(n);//add object to the gArray object
            d.cleanFunction(gList::clist);
            d.setShared(shared);

        }
        //! Destructor virtual
        virtual ~gList(){

        }
        //! Intended to be a search function but since not all type T have an operator == this would remain virtual.
        /** \param val: Value to search.
         * \return poitner to found element or 0 when fails.*/
        virtual T *search(const T &val,gu32 startPos=0,gu32 *indexOut=0,G_SEARCH_OPERATOR op=G_SEARCH_EQUAL){
            T *v;
            for(gu32 i=startPos;i<d.size();i++){
                v=pValue(i);
                if(searchValueOperator(*v,val,op)){
                    if(indexOut)*indexOut=i;
                    return v;
                }
            }
            return 0;
        }

      //Adding data to the list
        //! Inserts a value to list given a position on the array.
        /** Value is copied not referenced.
          \param val: Value to be inserted in the list.
          \param pos: 0 - Based index where value would be inserted. Current value on this position
          is displaced below */
        void insert(const T &val,gu32 pos)
        {
            dupc(val);
            T *n=a();
            *n=val;
            d.insert(n,pos);
        }
        //! Inserts another gList object of the same type given a position on the array.
        /** \param other: gList object to be added to the list.
            \param pos: start Position from which the object would be added.
            \param bCopy: Indicates if deep pointers are copied or just referenced */
       void insert(const gList<T> &other, gu32 pos,bool bCopy=true)
        {
           if(bCopy){
             gList<T> temp;
             //copy to a temporal gList object
             temp.copyFromOther(other,false);
             //make sure reference poitners are not removed when calling temp destructor
             temp.setRemoveReferences(false);
             //insert temp to d
             d.insert(temp.d,pos);
             return;//we are done here
           }
           d.insert(other.d,pos);
        }

        //Direct references reference pointer must have been created with the
       //! Inserts a reference to a object T given a position on the list.
       /** Values are referenced not copied.
         \param ref: Reference pointer added to the list. Make sure either to call
         gList::setRemoveReferences(false) if you are planing to keep the reference pointers
         when the list gets destroyed, or else the list would do it by itself.
         \param pos: Position on the list where ref would be inserted */
       void insert(const T *ref,gu32 pos)
       {
           dupc(val);
           d.insert(ref,pos);
       }

       //! Appends or adds a value at the end o the list.
       /** \param val: Value to be added at the end of the list. Value is copied not referenced */
        void append(const T &val)
        {
           dupc(val);
            T *n=a();

            *n=val;
            d.insertAtEnd(n);
        }
        //! Appends or adds another list at end of the list.
        /** \param other: List to be appended.
            \param bCopy: Indicates if each item on other gets copied or not */
        void append(const gList<T> &other,bool bCopy=true)
        {
            if(bCopy){
                gList<T> temp;
                temp.copyFromOther(other,false);
                temp.setRemoveReferences(false);
                d.insertAtEnd(temp.d);
                return;
            }
            d.insertAtEnd(other.d);
        }

        //! Appends a reference pointer of type T at the end of the list.
        /** \param ref: Reference pointer of type T to be added to the end of the list */
        void append(const T *ref)
         {
            dupc(val);
            d.insertAtEnd(ref);
        }

    //Data removal
        //! Removes an list item (or list items) from the list.
        /** \param pos: Position of item to be removed.
            \param blockSize: Number of items to be removed from the list starting from pos.
            If the clean up function has been set to object gList::d then reference pointers
            of each item from the lis would be removed too. */
        void remove(gu32 pos,gu32 blockSize)
        {
            d.removeAt(pos,blockSize,true);
        }
        //! Removes a list or list elements from the list starting from a given element.
        /** \param element: Element to be removed and from which it will star removing if the block is greather than one.
         *  \param blockSize: Quantity of items to be removed from the list, starting from the position where element was found.
         *  if the clean up function has been set to the object then reference pointers of item form the list would be removed too.*/
        void remove(T *element,gu32 blockSize){
            d.removeAt(element,blockSize,true);
        }

    //Operators
        //! Assigns or copies a another list to this list.
        /** \param other: gList to be assigned to this object.
            \return Reference to this object. */
        gList<T> &operator=(const gList<T> &other)
        {
            d=other.data();
            return *this;
        }
        //! Convenience operator that allows using the gList object as C-Style array.
        /** This is the const version.
          \param index: 0 based address of list item.
          \return const reference object of selected item.
          Beware: If list was initialized with alloc and without filling it with valid
          pointers of type reference poitner could point to 0 */
        const T &operator[](gu32 index)const
        {
            T *v;
            v=(T *)d[index];
            return (const T&)*v;
        }
        //! Convenience operator that allows using the gList object as C-Style array.
        /** This is the modifiable version.
          \param index: 0 based address of list item.
          \return const reference object of selected item.
          If list item pointed by index is 0 or has not been initialized.
          This function allocates data for this index an adds it to the list.
          So this function would return 0 if either the list is empty or the index is out of range.
         */
        T &operator[](gu32 index)
        {
            T *v=0;
            v=(T *)d[index];
            if(!v && !d.isEmpty() && index<d.size())
            {
                v=a();
                d.setData(v,index);
            }

            return *v;

        }
        //! Returns the reference pointer from the list selected by an index.
        /** \param index: Index of item to be retrieved from the list.
            \return valid pointer of type T or zero if list item has not been initialized,
            list is empty, or index is out of range */
        T *pValue(gu32 index) const
        {
            T *v;
            v=(T *)d[index];

            return v;
        }
        //! Retrieves an item from the list.
        /** This is the modifiable version.This function as the same implementation as operator[]
          \param index: 0 based address of list item.
          \return const reference object of selected item.
          If list item pointed by index is 0 or has not been initialized.
          This function allocates data for this index an adds it to the list.
          So this function would return 0 if either the list is empty or the index is out of range.
         */
        T &value(gu32 index)
        {
            T *v=0;
            v=(T *)d[index];
            if(!v && !d.isEmpty() && index<d.size())
            {
                v=a();
                d.setData(v,index);
            }

            return *v;
        }
        //! Retrieves an item from the list.
        /** This is the modifiable version.This function as the same implementation as operator[]
          \param index: 0 based address of list item.
          \return const reference object of selected item.
          Beware: If list was initialized with alloc and without filling it with valid
          pointers of type reference poitner could point to 0 */
        const T& value(gu32 index) const{
            T *v;
            v=(T *)d[index];


            return (const T&)*v;
        }
        //! Gets the total count of items contained in the list.
        /** \return Total items contained in the list 0 if empty.*/
        gu32 size() const
        {
            return d.size();
        }
        //! Returns if the list empty or not.
        /** \return true if list is empty. */
        bool isEmpty() const
        {
            return d.isEmpty();
        }
        //! Returns if the list locked or not.
        /** \return true if list is locked. */
        bool isLocked() const
        {
            return d.isLocked();
        }
        //! Locks the list for writing
        void lock(){d.lock();}
        //! Unlocks the list.
        void unlock(){d.unlock();}
        //! Sets the value of type T to the gList given an index.
        /** \param value: Value to be set to list at position index. Value is copied not referenced.
            \param index: 0 based index pointing the list position wheren value will be set.
            */
        void setValue(const T &value,gu32 index)
        {
            //Although gArray perform this check we need to check it again to avoid setting invalid indexes
            if(index>=d.size())return;//really necessary?
            T *n=(T *)d.getData(index);
            if(!n)//if value was not set we allocate mem for an new value and set it to the list
            {
                n=a();
                *n=value;
                d.setData(n,index);
            }
            else
            {
                *n=value;//value existed we only assing the new value to current item
            }
        }
        //! Sets the value of type T as reference to the list given an index.
        /** \param value: Value to be set to list at position index. Value is referenced not copied.
            \param index: 0 based index pointing the list position wheren value will be set.
            NOTICE: Value passed is as reference and by defaul, the list take ownership of it.
            use setRemoveReferences(false) to make sure the list won't remove references at destruction
            */
        void setValue(T *value,gu32 index)
        {
           //Although gArray perform this check we need to check it again to avoid setting invalid indexes
           if(index>=d.size())return;
            d.setData(value,index);

        }
        //! Preallocates memory for the list.
        /** The internal array is created but all array items are set to 0. See gArray documentation for details.
          \param isize: Count of items the list will be holding.
          \return true if successful false if failed. */
        bool alloc(gu32 isize)
        {
            return d.alloc(isize);
        }
        //! Fills the gList with valid pointers of type T
        /** Use this function to make sure it contains valid pointers as list items.
          You can use this after calling gList::alloc */
        void fill()
        {
            T *v;
            for(gu32 i=0;i<d.size();i++)
            {
                v=a();
                d.setData(v,i);
            }
        }
        //! Resizes the internal array by expanding it or shrinking it.
        /** \param newSize: New absolute size to be set to the list.
          Old values of the list remain intact unless you shrink the list.
          If shrinked, all values located at the tail of the list will be removed calling the clean up function.
          If expanded all new list items are created uninitialized. */
        void resize(gu32 newSize)
        {
            if(d.isEmpty())
                d.alloc(newSize);
            else
                d.resize(newSize,true);
        }
        //! Removes every reference used by the gArray object including the private member.
        void clear()
        {
            d.clear();
            d.cleanFunction(gList::clist);
            d.allocFunction(gList::alist);
        }
        //! Removes only list data an all of its items. Private member remains intact.
        /** This function has no efect if the gArray object does not own its private member */
        void clearList(){
            d.clearArray();
        }
        //! Set the gList object as reference.
        /** \param bSet: If set true, the object automatically sets a flag tellings that the private member
          of gArray will be passed as reference on copy constructors or assign operators. Very handy, to pass
          data from one object to another without wasting time on copying deep pointers.*/
        void setShared(bool bSet){
            d.setShared(bSet);
        }
        //! Convenience function that allows deep copying another gList object of the same type.
        /** Use this function if you want to copy deep pointers
          \param other: gList source from where data will be copied.
          \param bOnlyRefs: If true, only reference pointers will be copied similar to operator =.
                            If false, all data will be deep copied */
        void copyFromOther(const gList<T> &other,bool bOnlyRefs=false)
        {
            //ok here copy the entire list with new items
            T *val;
            d.clear();
            d.alloc(other.size());
            if(bOnlyRefs){
                for(gu32 i=0;i<other.size();i++)
                {
                    val=other.pValue(i);
                    setValue(val,i);//copy only reference
                }
            }
            else{
                for(gu32 i=0;i<other.size();i++)
                {
                    val=other.pValue(i);
                    setValue(*val,i);//copy all data
                }
            }
        }
        //! Function that makes a binary search of an element T.
        /** \param element: Element of type T to search
         *  \return true if found else false. */
        bool contains(const T *element,gu32 *indexOut=0) const{
            return d.contains(element,indexOut);
        }
        //! Makes the search for the value of the element. This pure virtual as not all types T have operator ==.
        /** \param val: Value type T to search
         * \return true if found else false */
        bool contains(const T &val,gu32 *indexOut=0) const{
            for(gu32 i=0;i<d.size();i++){
                if(searchValueOperator(val,value(i),G_SEARCH_EQUAL)){
                    if(indexOut)*indexOut=i;
                    return true;
                }
            }
            return false;
        }
        //! Same as size
        gu32 count() const{
            return d.size();
        }
        //! Counts the total matches on the list.
        gu32 count(const T &val,gUIntArray *outIndexes=0,G_SEARCH_OPERATOR op=G_SEARCH_EQUAL) const{
            gu32 c=0;
            for(gu32 i=0;i<d.size();i++){
                if(searchValueOperator(val,value(i),op)){
                    if(outIndexes)outIndexes->insert(i);
                    c++;
                }
            }
            return c;
        }
        //! Counts the total matches on the list by pointers.
        gu32 count(const T *ref,gUIntArray *outIndexes=0,G_SEARCH_OPERATOR op=G_SEARCH_EQUAL) const{
            return d.count(ref,outIndexes,op);
        }
        //! Removes all from the list received as parameter.
        void removeDuplicates(bool bValues=true){
            gLinkedList list,copy;//to avoid resizing the list so many times

            T *val,*lval;
            bool bFound;
            list.first();
            for(gu32 i=0;i<d.size();i++){
                val=pValue(i);
                copy.append(val);
            }
            copy.first();
            if(bValues){

                for(gu32 i=0;i<d.size();i++){
                    val=pValue(i);

                    list.first();
                    bFound=false;
                    while((lval=(T *)list.data())!=0){
                        if(searchValueOperator(*val,*lval,G_SEARCH_EQUAL)){
                            bFound=true;
                            break;
                        }
                        list.next();
                    }
                    if(!bFound)
                        list.append(val);
                    else
                        copy.markForRemoval();
                    copy.next();
                }
            }else{
                for(gu32 i=0;i<d.size();i++){
                    val=pValue(i);
                    if(list.search(val)){
                        copy.markForRemoval();
                    }else{
                        list.append(val);
                    }
                    copy.next();
                }
            }
            //done filtering
            copy.setFunction(gList::clist);//setting clean up function for the list
            copy.removeMarked();//remove duplicates
            d.cleanFunction(0);
            d.clear();//clear this list
            d.cleanFunction(gList::clist);
            //recopy
            d.alloc(list.count());
            list.first();
            for(gu32 i=0;i<d.size();i++){
                val=(T *)list.data();
                setValue(val,i);
                list.next();
            }
            copy.setFunction(0);//make sure the list does not mess up on the destructor

        }
        //! Sets the search operators bool.
        virtual bool searchValueOperator(const T &term1,const T &term2,G_SEARCH_OPERATOR op) const{
            (void)term1;(void)term2;(void)op;
          return false;
        }

        //! Static function that works as the clean up function for type T pointers.
        /** This function is called if the gArray object was initialized with this clean up function.
          And each time member functions gList::clear or gList::clearList are called, this funciton will be called
          per list item. To prevent the gArray object to call this function, only call setRemoveReferences(false)
          or setCleanupFunction(0).
          \param data: Reference pointer of item to be removed.*/
        static void clist(void *data)
        {
            T *n=(T *)data;

            if(n)
                delete n;
        }
        //! Static functions that works as allocation function for type T pointers
        static void *alist(const void *other){
            //! other represents another object of type T
            if(other){
                return new T(*static_cast<const T *>(other));
            }else{
                return new T;
            }
        }

    private:
        inline bool dupCheck(const T &val) const{
            if(m_duplicity)return contains(val);
            return false;
        }
        inline bool dupCheck(const T *val) const{
            if(m_duplicity)return contains(val);
            return false;
        }

    };

} //End namespace
#endif // GLIST_H
