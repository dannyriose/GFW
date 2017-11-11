#ifndef GVECTOR2_H
#define GVECTOR2_H


#include "gMath.h"
namespace gfw {
    template <class T>
    class SHARED_GFW gVector2{
    public:
        T x;
        T y;
        gVector2():x(0),y(0){

        }
        gVector2(const gVector2<T> &other):x(other.x),y(other.y){

        }
        gVector2(T _x,T _y):x(_x),y(_y){

        }

        gVector2<T> &operator = (const gVector2<T> &other){
            x=other.x;y=other.y;
            return *this;
        }
        void set(T _x,T _y){
            x=_x;y=_y;
        }

        inline T &getX(){return x;}
        inline T &getY(){return y;}
        inline const T &getX() const{return x;}
        inline const T &getY() const{return y;}
        //Arithmetic operators
        //! Addition operator.
        /** \param other: Vector to sum to this vector.
         * \return reference to this object. */
        inline gVector2<T> &operator+=(const gVector2<T> &other){
            x+=other.x;y+=other.y;
            return *this;
        }
        //! Substraction operator.
        /** \param other: Vector to substract to this vector.
         * \return reference to this object. */
        inline gVector2<T> &operator-=(const gVector2<T> &other){
            x-=other.x;y-=other.y;
            return *this;
        }
        //! Multiplication operator.
        /** \param other: Vector to multiply to this vector.
         * \return reference to this object. */
        inline gVector2<T> &operator*=(const gVector2<T> &other){
            x*=other.x;y*=other.y;
            return *this;
        }
        //! Divison operator.
        /** \param other: Vector to divide to this vector.
         * \return reference to this object. */
        inline gVector2<T> &operator/=(const gVector2<T> &other){
            x/=other.x;y/=other.y;
            return *this;
        }
        //! Overloaded addition operator.
        /** \param value: Value to be added to all three members of the vector
         * \return reference to this object. */
        inline gVector2<T> &operator+=(T value){
            x+=value;y+=value;
            return *this;
        }
        //! Overloaded substraction operator.
        /** \param value: Value to subscract to all three members of the vector
         * \return reference to this object. */
        inline gVector2<T> &operator-=(T value){
            x-=value;y-=value;
            return *this;
        }
        //! Overloaded multiplication operator.
        /** \param value: Value to multiply to all three members of the vector
         * \return reference to this object. */
        inline gVector2<T> &operator*=(T value){
            x*=value;y*=value;
            return *this;
        }
        //! Overloaded division operator.
        /** \param value: Value to divide to all three members of the vector
         * \return reference to this object. */
        inline gVector2<T> &operator/=(T value){
            x/=value;y/=value;
            return *this;
        }
        //! Addition operator. Sum this vector with another and returns a new one.
        /** \param other: Vector to be added to this vector
         * \return new vector object.*/
        inline gVector2<T> operator + (const gVector2<T> &other) const{
            return gVector2<T>(x+other.x,y+other.y);
        }
        //! Substraction operator. Substracts this vector with another and returns a new one.
        /** \param other: Vector to be substracted to this vector
         * \return new vector object.*/
        inline gVector2<T> operator - (const gVector2<T> &other) const{
            return gVector2<T>(x-other.x,y-other.y);
        }
        //! Multiplication operator. Multiplies this vector with another and returns a new one.
        /** \param other: Vector to be multiplied to this vector
         * \return new vector object.*/
        inline gVector2<T> operator * (const gVector2<T> &other) const{
            return gVector2<T>(x*other.x,y*other.y);
        }
        //! Division operator. Divides this vector with another and returns a new one.
        /** \param other: Vector to be divided to this vector
         * \return new vector object.*/
        inline gVector2<T> operator / (const gVector2<T> &other) const{
            return gVector2<T>(x/other.x,y/other.y);
        }
        //! Overloaded Addition operator. Sum this vector with a value and returns a new one.
        /** \param other: Value to be added to this vector
           * \return new vector object.*/
        inline gVector2<T> operator + (T value) const{
            return gVector2<T>(x+value,y+value);
        }
        //! Overloaded Substraction operator. Substracts this vector with a value and returns a new one.
        /** \param other: Value to be substracted to this vector
           * \return new vector object.*/
        inline gVector2<T> operator - (T value) const{
            return gVector2<T>(x-value,y-value);
        }
        //! Overloaded Multiplication operator. Multiplies this vector with a value and returns a new one.
        /** \param other: Value to be multiplied to this vector
           * \return new vector object.*/
        inline gVector2<T> operator * (T value) const{
            return gVector2<T>(x*value,y*value);
        }
        //! Overloaded Division operator. Divides this vector with a value and returns a new one.
        /** \param other: Value to be divided to this vector
           * \return new vector object.*/
        inline gVector2<T> operator / (T value) const{
            return gVector2<T>(x/value,y/value);
        }
        //! Changes the sign of each member of the vector object.
        /** \return reference to this object.*/
        inline gVector2<T> &operator -(){
            x=-x;y=-y;
            return *this;
        }
        //! Operator Equal
        /** \return true if two vectors are equal */
        inline bool operator == (const gVector2<T> &other) const{
            return ((x == other.x) && (y == other.y));
        }
        //! Overloaded operator equal.
        /** \param value to compare.
         *  \return true if all three values from vector are equal to comparison value. */
        inline bool operator == (T value) const{
            return ((x==value) &&(y==value));
        }
        //! Operator Not Equal
        /** \return true if two vectors are not equal */
        inline bool operator != (const gVector2<T> &other) const{
            return ((x != other.x) || (y != other.y));
        }
        //! Overloaded operator not equal.
        /** \param value to compare.
         *  \return true if all three values from vector are not equal to comparison value. */
        inline bool operator != (T value) const{
            return ((x!=value) || (y!=value));
        }
        inline T magnitude() const{
            return gMath::squareRoot(x*x + y*y);
        }
        //! Gets a normalized version of this vector.
        /** \return normalized vector. */
        inline gVector2<T> normalized() const{
            T m=x*x + y*y;
            m=(T)gMath::reciprocalSquareRoot((gScalar)m);
            return gVector2<T>(x * m,y * m);
        }
        //! Normalizes this vector.
        /** \return reference to this vector. */
        inline gVector2<T> &normalize(){
            T m=x*x + y*y;
            m=(T)gMath::reciprocalSquareRoot((gScalar)m);
            x*=m;y*=m;
            return *this;
        }
        //! Gets the dot product of this vector with another.
        inline gVector2<T> dot(const gVector2<T> &other) const{
            return gVector2<T>(x * other.x + y*other.y );
        }
        inline T distanceFrom(const gVector2<T> &other) const{
            gVector2<T> r(x-other.x,y-other.y);
            return r.magnitude();
        }

    };
    typedef gVector2<gScalar> gVector2f;
    typedef gVector2<gu32> gVector2u;
}
#endif // GVECTOR2_H
