#ifndef GLISTBASE_H
#define GLISTBASE_H
#include <gString/gString.h>
namespace gfw{
    //! Utility structure helpful as base for id base list elements
    struct gListElement{
        //! String identification for this element
        gString sid;
        //! Destructor virtual
        virtual ~gListElement(){}
    };

    //! Utility class that works as base for lists that use string search base
    template <class T>
    class gListBase{
    protected:
        //! List declared for gListElement type
        gList<gListElement> m_list;
    public:
        //! Constructor does nothing
        gListBase(){}
        //! Sets a cleaning function
        gListBase(IClean func, IAlloc afunc = 0){
            m_list.setCleanupFunction(func);
            if(afunc)
                m_list.setConstructFunction(afunc);
        }

        //! Copy constructor
        gListBase(const gListBase<T> &other):m_list(other.m_list){

        }
        //! Destructor
        virtual ~gListBase(){

        }
        //! Allocates memory for the list
        bool alloc(gu32 _size){
            return m_list.alloc(_size);
        }

        //! Adds a new element to the list
        /** \param id: String id of the element.
         * \return pointer to new created element.*/
        T *append(const gString &id){
            T *e=search(id);
            if(e)return 0;
            e=new T();
            e->sid=id;
            m_list.append(e);
            return e;
        }
        T *append(T *element){
            m_list.append(element);
            return element;
        }

        T *insert(const gString &id,gu32 position){
            T *e=search(id);
            if(e)return 0;
            e=new T();
            e->sid=id;
            m_list.insert(e,position);
            return e;
        }
        T *insert(T *element,gu32 position){
            m_list.insert(element,position);
        }

        T *search(const gString &id,gu32 *index=0) const{
            gListElement *e;

            for(gu32 i=0;i<m_list.size();i++){
                e=m_list.pValue(i);
                if(e->sid==id){
                    if(index)*index=i;
                    return static_cast<T*>(e);
                }
            }
            return 0;
        }
        T *value(gu32 index) const{
            return static_cast<T*>(m_list.pValue(index));
        }
        void remove(const gString &id){
            gu32 index;
            if(search(id,&index)){
                m_list.remove(index,1);
            }
        }
        void remove(gu32 index){
            m_list.remove(index,1);
        }
        void remove(T *element){
           m_list.remove(element,1);
        }
        gu32 size() const{
            return m_list.size();
        }
        gu32 count() const{
            return m_list.size();
        }
        gu32 count(const gString &id,gUIntArray *indexesOut=0,G_SEARCH_OPERATOR op=G_SEARCH_EQUAL){
            gListElement *e;
            gu32 c=0;
            bool val;
            for(gu32 i=0;i<m_list.size();i++){
                e=m_list.pValue(i);
                G_OP_EQ(op,id,e->sid,val);
                if(val){
                    if(indexesOut)indexesOut->insert(i);
                    c++;
                }
            }
            return c;
        }
        gu32 count(const T *element,gUIntArray *indexesOut,G_SEARCH_OPERATOR op=G_SEARCH_EQUAL){
            return m_list.count(element,indexesOut,op);
        }

        virtual void clear(){
            m_list.clear();
        }
        void setRemoveReferences(bool bSet){
            m_list.setRemoveReferences(bSet);
        }
        bool contains(const T *element,gu32 *indexOut=0) const{
            return m_list.contains(element,indexOut);
        }
        bool contains(const gString &id,gu32 *indexOut=0) const{
            gListElement *e;
            for(gu32 i=0;i<m_list.size();i++){
                e=m_list.pValue(i);
                if(e->sid==id){
                    if(indexOut)*indexOut=i;
                    return true;
                }
            }
            return false;
        }

        const T &operator[](int index) const{
            T *v=static_cast<T*>(m_list.pValue(index));
            return *v;
        }
        T &operator[](int index){
            T *v=static_cast<T*>(m_list.pValue(index));
            return *v;
        }
        void setValue(T *element,gu32 index){
            m_list.setValue(element,index);
        }
        void setShared(bool bSet){
            m_list.setShared(bSet);
        }
        void resize(gu32 newSize){
            m_list.resize(newSize);
        }
        void setCleanFunction(IClean func){
            m_list.setCleanupFunction(func);
        }
        void setConstructFunction(IAlloc func){
            m_list.setConstructFunction(func);
        }

        void copyFromOther(const gListBase<T> &other){
            m_list.copyFromOther(other.m_list);
        }
        bool isEmpty() const{
            return m_list.isEmpty();
        }

    };
}
#endif // GLISTBASE_H
