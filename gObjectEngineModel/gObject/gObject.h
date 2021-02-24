#ifndef GOBJECT_H
#define GOBJECT_H
#include <gfw_global.h>
#include <gfw_export.h>
#include <gObjectEngineModel/gProperty/gProperty.h>
#include <gTimers/gTimer.h>


namespace gfw
{
//!
//! \brief The gObject class. This is one of the core objects of the Object engine model.
//! It represents an object with properties. This is with the purpose of having an open
//! programatic object model for any application.
//!
class SHARED_GFW gObject: public gSharedEngineModel
{
public:
    //!
    //! \brief Constructor
    //! \param _parent The base parent or owner inside the hierachy of objects contained by this object
    //! \param _type Type of object. By default a GEM_Object
    //!
    gObject(gObject *_parent = 0, gs32 _type = GEM_OBJECT);
    ~gObject();

    //!
    //! \brief Inserts a new property to the property list to be handled by this object.
    //! \param _sid The string ID identifying this property
    //! \param _data Initial data value
    //! \param name Optional name
    //! \param _iid Numeric ID identifying this property
    //! \return Pointer to new created property o 0 if failed.
    //!
    gProperty *addProperty(const gString _sid,
                           const gVariant _data,
                           const gString &name = gString(),
                           gs32 _iid = -1);
    //!
    //! \brief Inserts a property to the property list. It increases the reference of it by calling
    //! gProperty::grab.
    //! \param _property Pointer to the property to be added. It must be a valid object.
    //! \return It returns the same pointer of the property passed as parameter.
    //!
    gProperty *addProperty(gProperty *_property);
    //!
    //! \brief Gets the property given a numeric index.
    //! \param index based index pointing to the property inside the list of properties
    //! very much like doing _property = propertylist[_index]
    //! \return Pointer to the property or 0 if failed.
    //!
    gProperty *property(gu32 index) const;
    //!
    //! \brief Gets the property given the string ID of the property.
    //! \param _sid String ID of the property to be searched in.
    //! \param indexOut Pointer to unsigned integer variable to retrieve the current index of
    //! the property inside the list.
    //! \return Pointer to the property found or 0 if not found.
    //!
    gProperty *property(const gString &_sid, gu32 *indexOut = 0) const;
    //!
    //! \brief Gets the property from the list given the integer ID of the property.
    //! \param _iid Integer ID of the property to be searched in list.
    //! \param indexOut Pointer to unsigned integer variable to retreieve the current index of the
    //! property inside the list.
    //! \return Pointer to the property found or 0 if not found.
    //!
    gProperty *property(gs32 _iid, gu32 *indexOut = 0) const;
    //!
    //! \brief Removes a property from the property given the 0 based index position
    //! in the list.
    //! \param _index Integer value containing the index value. This is a 0 based index value.
    //!
    void removeProperty(gu32 _index);
    //!
    //! \brief Removes a property from the list of properties given a String ID
    //! \param _sid The String ID.
    //!
    void removeProperty(const gString &_sid);
    //!
    //! \brief Removes a property from the list given the pointer to the property object.
    //! This searches for the pointer inside the list.
    //! \param _property Pointer to the property to be removed.
    //!
    void removeProperty(gProperty *_property);
    //!
    //! \brief Removes a property from the list given the Integer id of the property.
    //! \param _iid Integer ID of the property
    //!
    void removePropertyByIID(gs32 _iid);

    gTimer *addTimer(const gString &_sid, gu32 _interval, gs32 _iid = -1);
    gTimer *addTimer(gTimer *_timer);
    gTimer *timer(gu32 _index) const;
    gTimer *timer(const gString &_sid, gu32 *indexOut = 0) const;
    gTimer *timer(gs32 _iid, gu32 *indexOut = 0) const;
    void removeTimer(gu32 _index);
    void removeTimer(const gString &_sid);
    void removeTimerByIID(gs32 _iid);
    void removeTimer(gTimer *_timer);

    bool addChild(gObject *_obj);
    gObject *child(gu32 _index) const;
    gObject *child(const gString &_sid, gu32 *indexOut = 0) const;
    gObject *child(gs32 _iid, gu32 *indexOut = 0);
    void removeChild(gObject *_obj);
    void removeChild(gu32 *index);
    void removeChildByIID(gs32 _iid);
    void removeChild(const gString &_sid);

    bool setParent(gObject *_parent);
    gObject *parent() const;



};
}
#endif // GOBJECT_H
