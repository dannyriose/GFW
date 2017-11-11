#ifndef GDIMENSION_H
#define GDIMENSION_H

#include <gBase/gtypes.h>


namespace gfw{

template <class T>
class gDimension2{
public:
    gDimension2():
        m_width(0),
        m_height(0){

    }
    virtual ~gDimension2(){

    }

    T &width(){return m_width;}
    const T &width() const {return m_width;}
    T & height(){return m_height;}
    const T &height() const {return m_height;}

    void setWidthHeight(T _width, T _height){
        m_width = _width;
        m_height= _height;
    }

    void setWidth(T _width){
        m_width = _width;
    }

    void setHeight(T _height){
        m_height = _height;
    }

    T area() {return m_width * m_height;}

protected:
    T m_width;
    T m_height;
};

template <class T>

class gDimension3:public gDimension2<T>{

public:
    gDimension3(): m_depth(0){

    }

    ~gDimension3(){

    }

    void setWidthHeightDepth(T _width,T _height,T _depth){
        this->m_width=_width;
        this->m_height = _height;
        m_depth = _depth;
    }

    void setDepth(T _depth){
        m_depth = _depth;
    }

    T &depth() {return m_depth;}
    const T &depth() const {return m_depth;}

    T volume(){return this->m_width * this->m_height * m_depth;}
  protected:
    T m_depth;
};

typedef gDimension2<gScalar> gDimension2s;
typedef gDimension2<gu32> gDimension2u;
typedef gDimension2<gs32> gDimension2i;

typedef gDimension3<gScalar> gDimension3s;
typedef gDimension3<gf32> gDimension3f;
typedef gDimension3<gu32> gDimension3u;
typedef gDimension3<gs32> gDimension3i;
}


#endif // GDIMENSION2_H
