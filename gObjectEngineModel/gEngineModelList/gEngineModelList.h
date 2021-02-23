#ifndef GENGINEMODELLIST_H
#define GENGINEMODELLIST_H
#include <gfw_global.h>
#include <gfw_export.h>
#include <gAlignedLists/gAlignedPtrList.h>


namespace gfw
{

template <class T>
//!
//! \brief The gEngineModelListAllocator works as an allocator for the template gEngineModelList
//!
class gEngineModelListAllocator: public gAllocator<T>
{
public:
    gEngineModelListAllocator()
    {

    }
    //!
    //! \brief Removes reference to the contained object
    //! \param ptr: pointer to object
    //!
    virtual void remove(T *ptr)
    {
        ptr->drop();
    }
};

template <class T>
//!
//! \brief The gEngineModelList base class list for the Object Engine Model
//!
class SHARED_GFW gEngineModelList: public gAlignedPtrList<T,gEngineModelListAllocator<T> >
{
public:
    gEngineModelList()
    {

    }
    gEngineModelList(const gEngineModelList<T> &other):gAlignedPtrList<T,gEngineModelListAllocator<T> >(other)
    {

    }
    ~gEngineModelList()
    {

    }
    //!
    //! \brief Overrides the search function from gAlignedPtrList. Function searches using the string
    //! id.
    //! \param _sid: String ID of the object to be searched
    //! \param indexOut: 0 based index of the object foundin the list
    //! \return pointer to object found during search
    //!
    virtual T *search(const gString &_sid,gu32 *indexOut = 0) const
    {
        T *v;
        for(gu32 i = 0; i<this->d->m_size;i++){
            v = this->value(i);
            if(v->stringID() == _sid){
                if(indexOut)*indexOut = i;
                return v;
            }
        }
        return 0;
    }
    //!
    //! \brief Overrides the search function from gAlignedPtrList class using the iid of the
    //! object
    //! \param _iid: Integer id of the object
    //! \param indexOut; 0 based index of the object found in the list
    //! \return pointer to object found during search
    //!
    virtual T *search(gs32 _iid, gu32 *indexOut = 0) const
    {
        T *v;
        for(gu32 i = 0; i<this->d->m_size;i++){
            v = this->value(i);
            if(v->iID() == _iid){
                if(indexOut)*indexOut = i;
                return v;
            }
        }
        return 0;
    }

};

}
#endif // GENGINEMODELLIST_H
