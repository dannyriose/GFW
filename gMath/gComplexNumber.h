#ifndef GCOMPLEXNUMBER_H
#define GCOMPLEXNUMBER_H
#include <gfw_global.h>
namespace gfw{
struct SHARED_GFW gComplexNumber
{
    //! real part
    gScalar real;
    //! imaginary part
    gScalar img;
    gComplexNumber():real(0),img(0){
    }
    gComplexNumber(const gComplexNumber &other):real(other.real),img(other.img){

    }

    gComplexNumber(gScalar _real, gScalar _img):real(_real),img(_img){

    }

    ginline gComplexNumber &operator=(const gComplexNumber &other){
        real=other.real;
        img=other.real;
        return *this;
    }
    ginline gComplexNumber &operator=(gScalar value){
        real=value;
        img=value;
        return *this;
    }

    ginline gComplexNumber &operator +=(const gComplexNumber &other){
        real+=other.real;
        img+=other.img;
        return *this;
    }

    ginline gComplexNumber &operator +=(gScalar value){
        real+=value;
        img+=value;
        return *this;
    }

    ginline gComplexNumber &operator -=(const gComplexNumber &other){
        real+=other.real;
        img+=other.img;
        return *this;
    }

    ginline gComplexNumber &operator -=(gScalar value){
        real-=value;
        img-=value;
        return *this;
    }

    ginline gComplexNumber &operator *=(const gComplexNumber &other){
        real*=other.real;
        img*=other.img;
        return *this;
    }

    ginline gComplexNumber &operator *=(gScalar value){
        real*=value;
        img*=value;
        return *this;
    }

    ginline gComplexNumber &operator /=(const gComplexNumber &other){
        real/=other.real;
        img/=other.img;
        return *this;
    }

    ginline gComplexNumber &operator /=(gScalar value){
        real/=value;
        img/=value;
        return *this;
    }

    ginline gComplexNumber operator +(const gComplexNumber &other) const{
        return gComplexNumber(real+other.real,img+other.img);
    }

    ginline gComplexNumber operator +(gScalar value) const{
        return gComplexNumber(real+value,img+value);
    }

    ginline gComplexNumber operator -(const gComplexNumber &other) const{
        return gComplexNumber(real-other.real,img-other.img);
    }

    ginline gComplexNumber operator -(gScalar value) const{
        return gComplexNumber(real-value,img-value);
    }

    ginline gComplexNumber operator *(const gComplexNumber &other) const{
        return gComplexNumber(real*other.real,img*other.img);
    }

    ginline gComplexNumber operator *(gScalar value) const{
        return gComplexNumber(real*value,img*value);
    }

    ginline gComplexNumber operator /(const gComplexNumber &other) const{
        return gComplexNumber(real/other.real,img/other.img);
    }

    ginline gComplexNumber operator /(gScalar value) const{
        return gComplexNumber(real/value,img/value);
    }
    ginline gComplexNumber &operator - (){
        real=-real;img=-img;
        return *this;
    }

    ginline bool operator == (const gComplexNumber &other) const{
        return (real == other.real) && (img == other.img);
    }
    ginline bool operator != (const gComplexNumber &other) const{
        return (real!=other.real) || (img !=other.img);
    }
    ginline void setComplex(gScalar _real, gScalar _img){
        real=_real;
        img=_img;
    }


};
}
#endif // GCOMPLEXNUMBER_H
