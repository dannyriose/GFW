#ifndef gComplexBufferBUFFER_H
#define gComplexBufferBUFFER_H
#include "gComplexNumber.h"
#include <gMemory/gBaseArray.h>
namespace gfw{


//! Utility class container of an array of complex numbers.
/** Moreover, implements all operations of complex arithmetics. */
class SHARED_GFW gComplexBuffer:public gBaseShared
{
public:
    //! Constructor
    gComplexBuffer();
    //! Copy Constructor
    gComplexBuffer(const gComplexBuffer &other);
    ~gComplexBuffer();

    //! Creates the array
    /** \param usize: Total count of the array
       \return true if successful */
    bool create(gu32 usize);
    //! Creates the object without array elements
    /** \return true if successful */
    bool create();
    //! Gets the pointer to the array.
    /** \return pointer to array. */
    gComplexNumber *data() const;
    //! Overloaded member that gets the pointer of an array element.
    /** \param index: Index of array element.
        \return pointer of array element. */
    gComplexNumber *data(gu32 index) const;

    gu32 size() const;

    gComplexNumber &operator[](int index);
    const gComplexNumber &operator[](int index) const;
    gComplexBuffer &operator=(const gComplexBuffer &other);

    //Arithmetics
    gComplexBuffer &operator+=(const gComplexBuffer &other);
    gComplexBuffer &operator-=(const gComplexBuffer &other);
    gComplexBuffer &operator*=(const gComplexBuffer &other);
    gComplexBuffer &operator/=(const gComplexBuffer &other);

    gComplexBuffer operator + (const gComplexBuffer &other) const;
    gComplexBuffer operator - (const gComplexBuffer &other) const;
    gComplexBuffer operator * (const gComplexBuffer &other) const;
    gComplexBuffer operator / (const gComplexBuffer &other) const;


    void setAllTo(gScalar real,gScalar img);
    void setTo(gu32 index,gScalar real,gScalar img);
    void sizeDivide();
    gComplexBuffer section(gu32 index,gu32 _size) const;
    void append(const gComplexBuffer &other);

    virtual bool isEmpty() const;

    //meanly useful conversors
    gByteArray toByteArray() const;
    gFloatArray toFloatArray() const;
    gDoubleArray toDoubleArray()const;

    //Static functions
     static gComplexBuffer zeros(gu32 _size);//create an array of zeros
     static gComplexBuffer fromByteArray(const gByteArray &array);
     static gComplexBuffer fromByteArray(const gu8 *array,gu32 _size);
     static gComplexBuffer fromScalarArray(const gScalarArray &array);
    //complex multiplication
     gComplexBuffer cMul(const gComplexBuffer &other) const;
     gComplexBuffer &cMulThis(const gComplexBuffer &other);


};
}
#endif // gComplexBufferBUFFER_H
