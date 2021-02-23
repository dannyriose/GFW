#ifndef GPROPERTY_H
#define GPROPERTY_H
#include <gfw_global.h>
#include <gfw_export.h>
#include <gObjectEngineModel/gSharedEngineModel/gSharedEngineModel.h>
#include <gVariant/gVariant.h>

namespace gfw
{
class SHARED_GFW gProperty: public gSharedEngineModel
{
public:
    //!
    //! \brief gProperty: Base constructor. We set the gSharedEngineModel as GEM_PROPERTY as type
    //!
    gProperty():gSharedEngineModel(GEM_PROPERTY)
    {

    }
    //! The destructor
    ~gProperty(){
    }
    //!
    //! \brief setData: Sets the data value of this property
    //! \param _data: Value in a variant structure
    //!
    void setData(const gVariant &_data)
    {
        m_data = _data;
    }
    //!
    //! \brief setName: sets the name of this property.
    //! \param _name: String value with the name
    //!
    void setName(const gString &_name)
    {
        m_name = _name;
    }
    //!
    //! \brief data: Gets the data of this property const version
    //! \return the data
    //!
    const gVariant &data() const
    {
        return m_data;
    }
    //!
    //! \brief data: Gets the data of this property. Modifiable version
    //! \return reference value of data
    //!
    gVariant &data()
    {
        return m_data;
    }
    //!
    //! \brief name: Gets the name of the property
    //! \return Reference to a gString object non modifiable
    //!
    const gString &name() const
    {
        return m_name;
    }
private:
    //!
    //! \brief m_data: The data containing this property.
    //!
    gVariant m_data;
    //!
    //! \brief m_name: The optional name of this property
    //!
    gString m_name;

};
}


#endif // GPROPERTY_H
