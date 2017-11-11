#ifndef GVECTOR3_H
#define GVECTOR3_H
#ifdef WIN32
    #ifdef SHARED_LIBRARY
        #ifdef SHARED_EXPORT
            #define SHARED_GVECTOR3 __declspec(dllexport)
        #else
            #define SHARED_GVECTOR3 __declspec(dllimport)
        #endif
    #else
        #define SHARED_GVECTOR3
    #endif
#else
    #define SHARED_GVECTOR3
#endif

#include "gMath.h"

namespace gfw{
    //! Represents a 3D Vector in space
    template <class T>
    class gVector3{
    public:
        //! X coordinate
        T x;
        //! Y coordinate
        T y;
        //! Z coordiante
        T z;
        //! Common constructor
        gVector3():x(0),y(0),z(0){

        }
        //! Copy constructor
        gVector3(const gVector3<T> &other){
            x=other.x;y=other.y;z=other.z;
        }
        //! Constructor using all three coordinates to start
        gVector3(T _x,T _y,T _z):x(_x),y(_y),z(_z){

        }
        //! Sets the value of the vector receiving all three coordinates as parameters
        void set(T _x,T _y, T _z){
            x=_x;y=_y;z=_z;
        }
        inline T & getX(){return x;}
        inline const T &getX() const {return x;}
        inline T & getY(){return y;}
        inline const T &getY() const {return y;}
        inline T & getZ(){return z;}
        inline const T &getZ() const {return z;}

        gVector3<T> &operator=(const gVector3<T> &other){
            x=other.x;y=other.y;z=other.z;
            return *this;
        }
        //Arithmetic operators
        //! Addition operator.
        /** \param other: Vector to sum to this vector.
         * \return reference to this object. */
        inline gVector3<T> &operator+=(const gVector3<T> &other){
            x+=other.x;y+=other.y;z+=other.z;
            return *this;
        }
        //! Substraction operator.
        /** \param other: Vector to substract to this vector.
         * \return reference to this object. */
        inline gVector3<T> &operator-=(const gVector3<T> &other){
            x-=other.x;y-=other.y;z-=other.z;
            return *this;
        }
        //! Multiplication operator.
        /** \param other: Vector to multiply to this vector.
         * \return reference to this object. */
        inline gVector3<T> &operator*=(const gVector3<T> &other){
            x*=other.x;y*=other.y;z*=other.z;
            return *this;
        }
        //! Divison operator.
        /** \param other: Vector to divide to this vector.
         * \return reference to this object. */
        inline gVector3<T> &operator/=(const gVector3<T> &other){
            x/=other.x;y/=other.y;z/=other.z;
            return *this;
        }
        //! Overloaded addition operator.
        /** \param value: Value to be added to all three members of the vector
         * \return reference to this object. */
        inline gVector3<T> &operator+=(T value){
            x+=value;y+=value;z+=value;
            return *this;
        }
        //! Overloaded substraction operator.
        /** \param value: Value to subscract to all three members of the vector
         * \return reference to this object. */
        inline gVector3<T> &operator-=(T value){
            x-=value;y-=value;z-=value;
            return *this;
        }
        //! Overloaded multiplication operator.
        /** \param value: Value to multiply to all three members of the vector
         * \return reference to this object. */
        inline gVector3<T> &operator*=(T value){
            x*=value;y*=value;z*=value;
            return *this;
        }
        //! Overloaded division operator.
        /** \param value: Value to divide to all three members of the vector
         * \return reference to this object. */
        inline gVector3<T> &operator/=(T value){
            x/=value;y/=value;z/=value;
            return *this;
        }
        //! Addition operator. Sum this vector with another and returns a new one.
        /** \param other: Vector to be added to this vector
         * \return new vector object.*/
        inline gVector3<T> operator + (const gVector3<T> &other) const{
            return gVector3<T>(x+other.x,y+other.y,z+other.z);
        }
        //! Substraction operator. Substracts this vector with another and returns a new one.
        /** \param other: Vector to be substracted to this vector
         * \return new vector object.*/
        inline gVector3<T> operator - (const gVector3<T> &other) const{
            return gVector3<T>(x-other.x,y-other.y,z-other.z);
        }
        //! Multiplication operator. Multiplies this vector with another and returns a new one.
        /** \param other: Vector to be multiplied to this vector
         * \return new vector object.*/
        inline gVector3<T> operator * (const gVector3<T> &other) const{
            return gVector3<T>(x*other.x,y*other.y,z*other.z);
        }
        //! Division operator. Divides this vector with another and returns a new one.
        /** \param other: Vector to be divided to this vector
         * \return new vector object.*/
        inline gVector3<T> operator / (const gVector3<T> &other) const{
            return gVector3<T>(x/other.x,y/other.y,z/other.z);
        }
        //! Overloaded Addition operator. Sum this vector with a value and returns a new one.
        /** \param other: Value to be added to this vector
           * \return new vector object.*/
        inline gVector3<T> operator + (T value) const{
            return gVector3<T>(x+value,y+value,z+value);
        }
        //! Overloaded Substraction operator. Substracts this vector with a value and returns a new one.
        /** \param other: Value to be substracted to this vector
           * \return new vector object.*/
        inline gVector3<T> operator - (T value) const{
            return gVector3<T>(x-value,y-value,z-value);
        }
        //! Overloaded Multiplication operator. Multiplies this vector with a value and returns a new one.
        /** \param other: Value to be multiplied to this vector
           * \return new vector object.*/
        inline gVector3<T> operator * (T value) const{
            return gVector3<T>(x*value,y*value,z*value);
        }
        //! Overloaded Division operator. Divides this vector with a value and returns a new one.
        /** \param other: Value to be divided to this vector
           * \return new vector object.*/
        inline gVector3<T> operator / (T value) const{
            return gVector3<T>(x/value,y/value,z/value);
        }
        //! Changes the sign of each member of the vector object.
        /** \return reference to this object.*/
        inline gVector3<T> &operator -(){
            x=-x;y=-y;z=-z;
            return *this;
        }
        //! Operator Equal
        /** \return true if two vectors are equal */
        inline bool operator == (const gVector3<T> &other) const{
            return ((x == other.x) && (y == other.y) && (z == other.z));
        }
        //! Overloaded operator equal.
        /** \param value to compare.
         *  \return true if all three values from vector are equal to comparison value. */
        inline bool operator == (T value) const{
            return ((x==value) &&(y==value) && (z==value));
        }
        //! Operator Not Equal
        /** \return true if two vectors are not equal */
        inline bool operator != (const gVector3<T> &other) const{
            return ((x != other.x) || (y != other.y) || (z != other.z));
        }
        //! Overloaded operator not equal.
        /** \param value to compare.
         *  \return true if all three values from vector are not equal to comparison value. */
        inline bool operator != (T value) const{
            return ((x!=value) || (y!=value) || (z!=value));
        }
        inline bool operator < (const gVector3<T> &other) const{
            return (x < other.x || y < other.y  || z < other.z) && !(x > other.x || y > other.y || z > other.z);
        }
        inline bool operator > (const gVector3<T> &other) const{
            return (x > other.x || y > other.y  || z > other.z) && !(x < other.x || y < other.y || z < other.z);
        }
        inline T magnitude() const{
            return (T)gMath::squareRoot(x*x + y*y + z*z);
        }
        inline T magnitudeXY() const{
            return (T)gMath::squareRoot(x*x + y*y);
        }
        inline T magnitudeXZ() const{
            return (T)gMath::squareRoot(x*x + z*z);
        }
        inline T magnitudeYZ() const{
            return (T)gMath::squareRoot(y*y + z*z);
        }
        //! Gets a normalized version of this vector.
        /** \return normalized vector. */
        inline gVector3<T> normalized() const{
            T length=(x * x + y * y + z * z);
            if(!length)return gVector3<T>();
            length=(T)gMath::reciprocalSquareRoot(length);
            return gVector3<T>(x * length,y * length,z * length);
        }
        //! Normalizes this vector.
        /** \return reference to this vector. */
        inline gVector3<T> &normalize(){
            T length=(x * x + y * y + z * z);
            if(!length) *this;
            length=gMath::reciprocalSquareRoot(length);
            x*=length;y*=length;z*=length;
            return *this;
        }
        //! Gets the dot product of this vector with another.
        inline T dot(const gVector3<T> &other) const{
            return (x * other.x + y*other.y + z*other.z);
        }
        //! Gets the cross product of this vector with another.
        inline gVector3<T> cross(const gVector3<T> &other) const{
            return gVector3<T>(y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x);
        }
        //! Gets the distance of this vector with another
        inline gf32 distanceFrom(const gVector3<T> &other) const{
            gVector3<T> r(x - other.x,y - other.y,z-other.z);
            return r.magnitude();
        }

    };

    class SHARED_GVECTOR3 gVector3f:public gVector3<gScalar>{
    public:
        gVector3f(){

        }
        gVector3f(const gVector3f &other):gVector3<gScalar>(other){

        }
        gVector3f(gScalar _x,gScalar _y,gScalar _z):gVector3<gScalar>(_x,_y,_z){

        }
    };
}

#endif // GVECTOR3_H
