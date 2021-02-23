#ifndef GSHAREDENGINEMODEL_H
#define GSHAREDENGINEMODEL_H
#include <gfw_global.h>
#include <gfw_export.h>
#include <gString/gString.h>
#include "gEngineModelObjectTypes.h"
namespace gfw
{
//!
//! \brief Base data structures for the gEngine model class set.
//! All objects derived meant to be allocated in memory and deallocated
//! either by the new operator or malloc
//!
class SHARED_GFW gSharedEngineModel: public gSharedObject
{
public:
    //!
    //! \brief gSharedEngineModel: Standard Constructor
    //! \param itype: recieves the type of object
    //!
    gSharedEngineModel(gs32 itype):m_type(itype),m_iid(-1)
    {

    }
    virtual ~gSharedEngineModel()
    {

    }
    //!
    //! \brief grab: Increment the reference count of this object
    //!
    void grab() const
    {
        refCount++;
    }
    //!
    //! \brief drop: decreases the reference point of this object and
    //! delete it if no references are found
    //! \return true if object was destroyed or false if refCount still greather than 0
    //!
    bool drop() const
    {
        refCount--;
        gassert(refCount < 0, "ERROR: gSharedObject::refCount must be greather than or equal 0");
        if(refCount == 0)
        {
            delete this;
            return true;
        }
        return false;
    }
    //!
    //! \brief setType: Sets the type of object.
    //! \param _type: Integer value of the type identifying the object
    //!
    void setType(gs32 _type)
    {
        m_type = _type;
    }
    //!
    //! \brief setStingID: Sets the String ID of the object.
    //! \param _sid: Reference to a string
    //!
    void setStingID(const gString &_sid)
    {
        m_sid = _sid;
    }
    //!
    //! \brief setIID: Sets the integer id of the object
    //! \param _iid: Integer data with ID value.
    //!
    void setIID(gs32 _iid)
    {
        m_iid = _iid;
    }
    //!
    //! \brief type: Gets the object type
    //! \return integer value of object type
    //!
    gs32 type() const
    {
        return m_type;
    }
    //!
    //! \brief stringID: Gets the object string ID
    //! \return String value of ID
    //!
    const gString &stringID() const
    {
        return m_sid;
    }
    //!
    //! \brief iID: Gets the object integer ID
    //! \return Value of ID
    //!
    gs32 iID() const
    {
        return m_iid;
    }
private:
    //!
    //! \brief The type of object. Used to tag derivates of this object
    //!
    gs32 m_type;
    //!
    //! \brief m_sid: The string id of the object. This is has the purpose of programatic behaviour
    //!
    gString m_sid;
    //!
    //! \brief m_iid: The integer ID of the object. This for programatic purposes of the object
    //!
    gs32 m_iid;
};
}
#endif // GSHAREDENGINEMODEL_H
