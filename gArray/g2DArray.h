/*Copyright (c) <2014> <''copyright holders''>

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you must not
   claim that you wrote the original software. If you use this software
   in a product, an acknowledgment in the product documentation would be
   appreciated but is not required.

   2. Altered source versions must be plainly marked as such, and must not be
   misrepresented as being the original software.

   3. This notice may not be removed or altered from any source
   distribution.*/
#ifndef G2DARRAY_H
#define G2DARRAY_H
#include "gList.h"

//WARNING: This class was created but never used. Need testing first.
namespace gfw
{
    //! Template class for handling dynamic 2D Arrays
    template <class T>
    class g2DArray
    {
        //! width of array
        int m_width;
        //! Height
        int m_height;
    public:
        //! Data of array contained on gList object
        gList<T> d;
        //! Constructor
        g2DArray():m_width(0),m_height(0){} //Do nothing
        //! Copy constructor
        /** \param other: Objects is initializated from other array. Noticed that ref pointers will be passed without creating new
                   memory for the array elements */
        g2DArray(const g2DArray<T> &other)
        {
            bool bp=other.d.data().isByPass();
            d=other.d;
            if(bp)d.setByPass(true);
        }
        //! Allocates memory for the array
        /** \param _width: Width of Array.
          \param _height: Height of Array.
          \return True if array was succesfully created or else false
          */
        bool alloc(int _width,int _height)
        {
            m_width=_width;
            m_height=_height;
            return d.alloc(m_width * m_height);
        }

        //! Clears or deallocates memory used by this array
        void clear()
        {
            d.clear();
        }
        //! Gets a non modifiable element from the array given coordinates x and y
        /** \param x: X Coordinate.
            \param y: Y Coordinate.
            \return Reference of array element. There is no error check here make sure the array is filled with valid data.

          */
        const T &value(int x, int y) const
        {
            return d.value(y*m_width + x);
        }

        //! Gets a modifiable element from the array given coordinates x and y
        /** \param x: X Coordinate.
          \param y: Y Coordinate.
          \return Reference of array element. There is no error check here make sure the array is filled with valid data.
          */
        T &value(int x, int y)
        {
            return d[y*m_width + x];
        }
        //! Gets a non modifiable element from the array given an index
        /** \param index: 0 based index containing the address of the element.
          \return Reference of array element. */
        const T &value(gu32 index) const
        {
            return d.value(index);
        }
        //! Gets a modifiable element from the array given an index
        /** \param index: 0 based index containing the address of the element.
          \return Reference of array element. */
        T &value(gu32 index)
        {
            return d.value(index);
        }
        //! Gets a reference pointer of an element from the array given coordinates x and y
        /** \param x: X Coordinate.
          \param y: Y Coordinate.
          \return Reference pointer from array. There is no error check here make sure the array is filled with valid data.
          */
        T *pvalue(int x,int y) const
        {
            return d.pValue(y*m_width + x);
        }
        //! Gets a reference pointer of an element from the array given an index
        /** \param index: 0 based index containing the address of the element.
          \return Reference pointer of array element. */
        T *pvalue(gu32 index) const
        {
            return d.pValue(index);
        }

        //! Sets a new value to the array given x and y coordnates. Old value is replaced by new one.
        /** \param x: X Coordinate of array.
            \param y: Y Coordinate of array.
            \param val: New value to be set into array coordinate. */
        void setValue(int x, int y, const T &val)
        {
            d.setValue(val,y*m_width + x);
        }
        /** Sets a new value as reference pointer to the array given x and y coordnates. Old value is replaced by new one
            notice the array would take ownership of memory address and will deallocate it when array is destroyed.
            \param x: X Coordinate of array.
            \param y: Y Coordinate of array.
            \param val: New value to be set into array coordinate. */
        void setValue(int x, int y, T *val)
        {
            d.setValue(val,y*m_width + x);
        }
        //! Fills array with valid values of type T.
        void fill()
        {
           d.fill();
        }
        //! Checks whether array contains any data
        //! \return True if array is empty else false
        bool isEmpty() const
        {
            return d.isEmpty();
        }
        //! Width of array
        //! \return width of array
        int width() const
        {
            return m_width;
        }
        //! Height of array
        //! \return height of array
        int height() const
        {
            return m_height;
        }

        //! Gets the size of the array or element count
        //! \return Element count of the array.
        gu32 size() const
        {
            d.size();
        }


    };
}

#endif // G2DARRAY_H
