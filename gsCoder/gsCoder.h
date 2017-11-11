#ifndef GSCODER_H
#define GSCODER_H

#include <gfw_global.h>
#include <gString/gString.h>


/*******************Implements a class for encoding and decoding strings*************/
namespace gfw
{
    //! Provides an easy interface to encript strings
    /** gsCode class is an utility that helps encripting strings.
      It works based on 16 bit tables. The basic encoding can be created using random
      generated tables. */
    class SHARED_GFW gsCoder:public gBaseShared
    {

    public:
        //! Constructor
        gsCoder();
        //! Copy constructor
        /** \param other gsCoder source to initialize object from */
        gsCoder(const gsCoder &other);
        //! Destructor
        ~gsCoder();
        //! Creates the encription table
        /** \param w: width of table.
            \param h: height of table.
            \return true if succesful. */
        bool t_init(gu32 w=256,gu32 h=256);//Creates the table
        //! Creates a 256*256 table with values.
        /** \param bRandom: If true, values on table are created randomly. The rand function is used just
          one time to get a seed from which the table will be filled using increments of 0xff.
            \param iRandomSeed: Sets the seed for the random number generator. */
        int t_base(bool bRandom, int iRandomSeed=0);//Creates a table and initializes it
        //! Set the encription key.
        /** \param _key: String containing the encription key. */
        void setKey(const gString &_key);//set the encoding key
        //get the key
        //! Gets the current encription key.
        /** \return reference string of the current encription key. */
        const gString &key() const;
        //! Sets values to encription table.
        /** \param x: X coordinate of table value.
            \param y: Y coordinate of table value.
            \param value: Value to set to the table */
        void t_setvalue(gu32 x,gu32 y,int value);

        //encoder version low case hex values
        //! Encodes a string using the current encription key.
        /** \param value: String to be encoded.
            \return encoded string. */
        gString sh_enc(const gString &value) const;
        //! Decodes a string using the current encription key.
        /** \param enc: Encoded string to be decoded.
            \param decoded string or empty if failed. */
        gString sh_dec(const gString &enc) const;
        //! Assings another gsCoder object to this object.
        gsCoder &operator = (const gsCoder &other);



    };
}

#endif // GSCODER_H
