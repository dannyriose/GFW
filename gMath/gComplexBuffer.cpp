
#include "impl/gComplexBufferPrivate.h"
using namespace gfw;
#define ca gComplexBufferPrivate *o=(gComplexBufferPrivate *)d
#define co gComplexBufferPrivate *io=(gComplexBufferPrivate *)other.d
#define cn if(!d) return

gComplexBuffer::gComplexBuffer():gBaseShared(new gComplexBufferPrivateFactory)
{
}

gComplexBuffer::gComplexBuffer(const gComplexBuffer &other):gBaseShared(other,new gComplexBufferPrivateFactory)
{

}

gComplexBuffer::~gComplexBuffer()
{
}

bool gComplexBuffer::create(gu32 usize)
{
    gComplexBufferPrivate *o;
    clear();

    o=new gComplexBufferPrivate;
    d=o;


    return o->data.alloc(usize);

}

bool gComplexBuffer::create()
{
    clear();
    d=new gComplexBufferPrivate;
    return d!=0;
}



gComplexNumber *gComplexBuffer::data() const
{
    cn 0;
    ca;
    return o->data.data();
}

gComplexNumber *gComplexBuffer::data(gu32 index) const
{
    cn 0;
    ca;
    gComplexNumber *b=o->data.data();
    assert(index<o->data.size());
    return &b[index];
}

gu32 gComplexBuffer::size() const
{
    cn 0;
    ca;
    return o->data.size();
}

gComplexBuffer &gComplexBuffer::operator =(const gComplexBuffer &other){
    assign(other);
    return *this;
}

gComplexNumber &gComplexBuffer::operator [](int index)
{
    gComplexNumber *b=0;
    cn *b;
    ca;
    return o->data[index];
}

const gComplexNumber &gComplexBuffer::operator [](int index) const
{
    cn *(gComplexNumber *)0;
    ca;
    return o->data[index];
}

//Aritmetics now
gComplexBuffer &gComplexBuffer::operator +=(const gComplexBuffer &other)
{
    gu32 i;
    assert(d!=0);
    assert(!other.isEmpty());
    ca;
    co;
    assert(o->data.size()==io->data.size());
    gComplexNumber *src,*dest;
    dest=o->data.data();
    src=io->data.data();
    for(i=0;i<o->data.size();i++)
    {
        //To get more fast
        dest[i].real+=src[i].real;
        dest[i].img+=src[i].img;
    }

    return *this;
}


gComplexBuffer &gComplexBuffer::operator -=(const gComplexBuffer &other)
{
    gu32 i;
    assert(d!=0);
    assert(!other.isEmpty());
    ca;
    co;
    assert(o->data.size()==io->data.size());
    gComplexNumber *src,*dest;
    dest=o->data.data();
    src=io->data.data();
    for(i=0;i<o->data.size();i++)
    {
        //To get more fast
        dest[i].real-=src[i].real;
        dest[i].img-=src[i].img;
    }

    return *this;
}


gComplexBuffer &gComplexBuffer::operator *=(const gComplexBuffer &other)
{
    gu32 i;
    assert(d!=0);
    assert(!other.isEmpty());
    ca;
    co;
    assert(o->data.size()==io->data.size());
    gComplexNumber *src,*dest;
    dest=o->data.data();
    src=io->data.data();
    for(i=0;i<o->data.size();i++)
    {
        //To get more fast
        dest[i].real*=src[i].real;
        dest[i].img*=src[i].img;
    }

    return *this;
}


gComplexBuffer &gComplexBuffer::operator /=(const gComplexBuffer &other)
{
    gu32 i;
    assert(d!=0);
    assert(!other.isEmpty());
    ca;
    co;
    assert(o->data.size()==io->data.size());
    gComplexNumber *src,*dest;
    dest=o->data.data();
    src=io->data.data();
    for(i=0;i<o->data.size();i++)
    {
        //To get more fast
        if(src[i].real!=0)
            dest[i].real/=src[i].real;
        if(src[i].img!=0)
            dest[i].img/=src[i].img;
    }

    return *this;
}

gComplexBuffer gComplexBuffer::operator +(const gComplexBuffer &other) const
{
    gComplexBuffer ret;
    ret=*this;
    ret+=other;
    ret.setShared(true);
    return gComplexBuffer(ret);
}

gComplexBuffer gComplexBuffer::operator -(const gComplexBuffer &other) const
{
    gComplexBuffer ret;
    ret=*this;
    ret-=other;
    ret.setShared(true);
    return gComplexBuffer(ret);
}

gComplexBuffer gComplexBuffer::operator *(const gComplexBuffer &other) const
{
    gComplexBuffer ret;
    ret=*this;
    ret*=other;
    ret.setShared(true);
    return gComplexBuffer(ret);
}

gComplexBuffer gComplexBuffer::operator /(const gComplexBuffer &other) const
{
    gComplexBuffer ret;
    ret=*this;
    ret/=other;
    ret.setShared(true);
    return gComplexBuffer(ret);
}


gComplexBuffer gComplexBuffer::zeros(gu32 _size)
{
    gComplexBuffer ret;
    ret.create(_size);
    ret.setAllTo(0,0);
    ret.setShared(true);
    return gComplexBuffer(ret);
}

void gComplexBuffer::setAllTo(gScalar real, gScalar img)
{
    cn;
    ca;
    gu32 i;
    assert(!isEmpty());
    gComplexNumber *b=o->data.data();

    for(i=0;i<o->data.size();i++)
    {
        b[i].img=img;
        b[i].real=real;
    }
}

void gComplexBuffer::setTo(gu32 index, gScalar real, gScalar img)
{
    cn;
    ca;
    assert(!isEmpty());
    assert(index<o->data.size());
    gComplexNumber *b=o->data.data();

    b[index].real=real;
    b[index].img=img;
}


bool gComplexBuffer::isEmpty() const
{
    cn true;
    ca;

    return o->data.isEmpty();
}


gByteArray gComplexBuffer::toByteArray() const
{
    cn gByteArray();
    assert(!isEmpty());
    ca;
    gu32 i;
    gByteArray ret;
    gComplexNumber *b;
    gu8 *c;


    ret.alloc(o->data.size());

    b=o->data.data();
    c=ret.data();
    for(i=0;i<o->data.size();i++)
    {
        c[i]=gu8(b[i].real);
    }
    ret.setShared(true);
    return gByteArray(ret);
}

gComplexBuffer gComplexBuffer::fromByteArray(const gByteArray &array)
{
    if(array.isEmpty())return gComplexBuffer();
    gComplexBuffer ret;
    gu32 i;
    gu8 *bytes;
    gComplexNumber *b;
    ret.create(array.size());

    bytes=array.data();
    b=ret.data();
    for(i=0;i<array.size();i++)
    {
        b[i].real=gScalar(bytes[i]);
        b[i].img=0;
    }

    ret.setShared(true);
    return gComplexBuffer(ret);

}

gComplexBuffer gComplexBuffer::fromByteArray(const gu8 *array, gu32 _size)
{
    gComplexBuffer ret;
    gu32 i;
    gComplexNumber *b;
    assert(array!=0);
    assert(_size!=0);
    ret.create(_size);

    b=ret.data();
    for(i=0;i<_size;i++)
    {
        b[i].real=gScalar(array[i]);
        b[i].img=0;
    }

    ret.setShared(true);
    return gComplexBuffer(ret);
}

gComplexBuffer gComplexBuffer::fromScalarArray(const gScalarArray &array)
{
    if(array.isEmpty())return gComplexBuffer();
    gComplexBuffer ret;
    gu32 i;
    const gScalar *scalar;
    gComplexNumber *b;
    ret.create(array.size());

    scalar=array.data();
    b=ret.data();
    for(i=0;i<array.size();i++)
    {
        b[i].real=scalar[i];
        b[i].img=0;
    }

    ret.setShared(true);
    return gComplexBuffer(ret);
}

void gComplexBuffer::sizeDivide()
{
    assert(!isEmpty());
    gu32 i,_size;
    gComplexNumber *b;
    ca;
    b=o->data.data();
    _size=o->data.size();
    for(i=0;i<_size;i++)
    {
        b[i].real/=gScalar(_size);
        b[i].img/=gScalar(_size);
    }
}


gComplexBuffer gComplexBuffer::section(gu32 index, gu32 _size) const
{
    assert(!isEmpty());
    ca;
    gComplexBuffer other;

    other.create();

    co;

    io->data=o->data.section(index,_size);

    other.setShared(true);
    return gComplexBuffer(other);

}

void gComplexBuffer::append(const gComplexBuffer &other)
{
    assert(!other.isEmpty());
    if(!d)
    {
        *this=other;
        return;
    }
    ca;
    co;

    o->data.insert(io->data);
}

gFloatArray gComplexBuffer::toFloatArray() const
{
    cn gFloatArray();
    assert(!isEmpty());
    ca;
    gu32 i;
    gFloatArray ret;
    gComplexNumber *b;
    float *c;


    ret.alloc(o->data.size());

    b=o->data.data();
    c=ret.data();
    for(i=0;i<o->data.size();i++)
    {
        c[i]=float(b[i].real);
    }
    ret.setShared(true);
    return gFloatArray(ret);
}

gDoubleArray gComplexBuffer::toDoubleArray() const
{
    cn gDoubleArray();
    assert(!isEmpty());
    ca;
    gu32 i;
    gDoubleArray ret;
    gComplexNumber *b;
    double *c;


    ret.alloc(o->data.size());

    b=o->data.data();
    c=ret.data();
    for(i=0;i<o->data.size();i++)
    {
        c[i]=double(b[i].real);
    }
    ret.setShared(true);
    return gDoubleArray(ret);
}

//Complex multiplication
gComplexBuffer &gComplexBuffer::cMulThis(const gComplexBuffer &other)
{
    assert(!isEmpty());
    assert(!other.isEmpty());
    gu32 i;
    ca;
    co;
    assert(o->data.size()==io->data.size());
    gComplexNumber *thisVector;
    gComplexNumber *otherVector;

    thisVector=o->data.data();
    otherVector=io->data.data();
    for(i=0;i<o->data.size();i++)
    {
        thisVector[i].img=(thisVector[i].real * otherVector[i].img) +
                (thisVector[i].img * otherVector[i].real);
        thisVector[i].real=(thisVector[i].real * otherVector[i].real) -
                (thisVector[i].img * otherVector[i].img);
    }

    return *this;

}

gComplexBuffer gComplexBuffer::cMul(const gComplexBuffer &other) const
{
    assert(!isEmpty());
    assert(!other.isEmpty());
    gu32 i;
    ca;
    co;
    assert(o->data.size()==io->data.size());
    gComplexNumber *thisVector;
    gComplexNumber *otherVector;
    gComplexNumber *final;
    gComplexBuffer ret;

    ret.create(o->data.size());
    thisVector=o->data.data();
    otherVector=io->data.data();
    final=ret.data();
    for(i=0;i<o->data.size();i++)
    {
        final[i].img=(thisVector[i].real * otherVector[i].img) +
                (thisVector[i].img * otherVector[i].real);
        final[i].real=(thisVector[i].real * otherVector[i].real) -
                (thisVector[i].img * otherVector[i].img);
    }

    ret.setShared(true);
    return gComplexBuffer(ret);
}
