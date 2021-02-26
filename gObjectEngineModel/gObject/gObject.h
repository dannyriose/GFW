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
    //! \return True if succesfully removed or false if failed.
    //!
    bool removeProperty(gu32 _index);
    //!
    //! \brief Removes a property from the list of properties given a String ID
    //! \param _sid The String ID.
    //! \return True if succesfully removed or false if failed.
    //!
    bool removeProperty(const gString &_sid);
    //!
    //! \brief Removes a property from the list given the pointer to the property object.
    //! This searches for the pointer inside the list.
    //! \param _property Pointer to the property to be removed.
    //! \return True if succesfully removed or false if failed.
    //!
    bool removeProperty(gProperty *_property);
    //!
    //! \brief Removes a property from the list given the Integer id of the property.
    //! \param _iid Integer ID of the property
    //! \return True if succesfully removed or false if failed.
    //!
    bool removePropertyByIID(gs32 _iid);
    //!
    //! \brief Gets the count of properties contained by the object
    //! \return Integer value with property count,
    //!
    gu32 propertyCount() const;
    //!
    //! \brief Creates a new timer to the object. This can be virtual or system timer
    //! \param _sid The string ID of the timer
    //! \param _interval The interval of the timer. The time elapsed to get triggered
    //! \param _iid The timer integer ID
    //! \return The pointer to a new created timer or 0 if failed.
    //!
    gTimer *addTimer(const gString &_sid,
                     gu32 _interval,
                     gs32 _iid = -1,
                     gs32 _timerType = GTIMER_TYPE_VIRTUAL);
    //!
    //! \brief Inserts a new timer to the timer list.
    //! \param _timer The timer pointer to be added.
    //! \return 0 if failed true the timer pointer (same as parameter)
    //!
    gTimer *addTimer(gTimer *_timer);
    //!
    //! \brief Gets a timer from the timer list.
    //! \param _index: 0 based index of timer in list.
    //! \return Pointer to timer object
    //!
    gTimer *timer(gu32 _index) const;
    //!
    //! \brief Gets a timer given its string ID
    //! \param _sid The string id of the timer object to get
    //! \param indexOut Optional parameter to get the 0 based index of the timer
    //! found on list.
    //! \return Pointer to timer object found or 0 if not found.
    //!
    gTimer *timer(const gString &_sid, gu32 *indexOut = 0) const;
    //!
    //! \brief Gets a timer given its Integer ID.
    //! \param _iid Integer (Positive value) of timer object to find.
    //! \param indexOut Optional parameter to store the 0 based index of the
    //! timer object found. This remains the same value it had if not found.
    //! if pointer is NULL then this gets ignored. Must point to a valid integer
    //! variable.
    //! \return Desired timer object pointer or 0 if failed.
    //!
    gTimer *timer(gs32 _iid, gu32 *indexOut = 0) const;
    //!
    //! \brief Removes a timer from the object.
    //! \param _index 0 based index of the timer in the object.
    //! \return True if succesfully removed or false if failed.
    //!
    bool removeTimer(gu32 _index);
    //!
    //! \brief Finds and removes a timer from the Object given its String ID.
    //! \param _sid String ID of timer to find in object.
    //! \return True if succesfully removed or false if failed.
    //!
    bool removeTimer(const gString &_sid);
    //!
    //! \brief Finds and removes a timer from the Object given its integer ID.
    //! \param _iid Integer ID of timer to find in object.
    //! \return True if succesfully removed or false if failed.
    //!
    bool removeTimerByIID(gs32 _iid);
    //!
    //! \brief  Finds and removes a timer from the Object given the timer object itself.
    //! \param _timer Pointer to timer object to find and remove
    //! \return True if succesfully removed or false if failed.
    //!
    bool removeTimer(gTimer *_timer);
    //!
    //! \brief Gets the total count timers the object contains.
    //! \return Integer value of the total count of timers.
    //!
    gu32 timerCount() const;
    //!
    //! \brief Adds a child object to the object.
    //! \param _obj gObject object to add as a child of this object. It increases
    //! the reference count of the new child object by calling gObject::grab
    //! \return True if added to child list or false if failed. It fails when there are
    //! duplicates.
    //!
    bool addChild(gObject *_obj);
    //!
    //! \brief Gets a child object from the internal list.
    //! \param _index 0 based Index of desired object to retrieve.
    //! \return Poitner to desired object to retrieve or 0 if failed. It fails if object
    //! already exists as child
    //!
    gObject *child(gu32 _index) const;
    //!
    //! \brief Gets a child object contained in this object by giving an id as parameter.
    //! \param _sid String ID of object to find in list.
    //! \param indexOut Pointer to variable to store the 0 based index value where the object was found.
    //! If this paremeter is NULL (or 0) then this parameter gets ignore.
    //! \return Pointer to desired object to find in the child list or 0 if failed.
    //!
    gObject *child(const gString &_sid, gu32 *indexOut = 0) const;
    //!
    //! \brief Gets a child object contained in this object by giving its id as parameter.
    //! \param _iid Integer ID of object to find in list.
    //! \param indexOut Pointer to unsigned integer variable to store the 0 based index where the object was found.
    //! If this parameter is NULL or 0, then it is ignored.
    //! \return Pointer to object found in the child list or 0 if failed.
    //!
    gObject *child(gs32 _iid, gu32 *indexOut = 0);
    //!
    //! \brief Removes a child object from this object.
    //! \param _obj Child object to be removed from list.
    //! \return True if succesfully removed or false if failed.
    //!
    bool removeChild(gObject *_obj);
    //!
    //! \brief Removes a child object from the object.
    //! \param _index Integer value with 0 based index of child object to remove.
    //! \return True if succesfully removed or false if failed.
    //!
    bool removeChild(gu32 _index);
    //!
    //! \brief Removes a child from the object by giving an ID as parameter.
    //! \param Integer ID value of child object to remove.
    //! \return True if succesfully removed or false if failed.
    //!
    bool removeChildByIID(gs32 _iid);
    //!
    //! \brief Removes a child from the object by giving a String ID as parameter.
    //! \param _sid String ID of child object to remove.
    //! \return True if succesfully removed or false if failed.
    //!
    bool removeChild(const gString &_sid);

    //!
    //! \brief Sets the Parent object. It increases the reference count and auto is auto added to the parent
    //! child list.
    //! \param _parent New Parent of this object.
    //! \return True if set or false if failed. It fails if object is already part of the child list.
    //!
    bool setParent(gObject *_parent);
    //!
    //! \brief Gets the current parent object.
    //! \return Pointer of parent object.
    //!
    gObject *parent() const;



};
}
#endif // GOBJECT_H
