#ifndef GBASEARRAY_H
#define GBASEARRAY_H

#include "gMemory.h"
namespace gfw
{
    template <class T>
    class gBaseArray{
    protected:
        gMemory d;
    public:

        gBaseArray(){

        }
        //Copy constructor
        gBaseArray(const gBaseArray<T> &other):d(other.d){
        }
        gBaseArray(const gMemory &mem):d(mem){

        }

        gBaseArray(const T *data,gu32 _size,bool bCopy=true){
            d.setData(data,sizeof(T),_size,bCopy);
        }
        gBaseArray(const T &initialValue){
            d.setData(&initialValue,sizeof(T),1,true);
        }

        void setShared(bool bSet){
            d.setShared(bSet);
        }

        bool alloc(gu32 _size){
            return  d.alloc(sizeof(T),_size,0);
        }

        void clear(){
            d.clear();
        }
        T &operator [] (int index){
            return *(T *)d.data(index);
        }


        const T &operator [] (int index) const{
            return *(T *)d.data(index);
        }

        T *data() const{
            return (T *)d.data();
        }
        operator const T*() const{
            return (T *)d.data();
        }

        gBaseArray<T> &operator=(const gBaseArray<T> &other)
        {
            d=other.d;
            return *this;
        }

        void setMemTo(gu8 value){
            d.memsetTo(value);
        }
        gu32 size() const{
            return d.size();
        }

        bool isEmpty() const{
            return d.isEmpty();
        }

        void insert(const T *array,gu32 _size){
            d.insertAtEnd(array,_size,sizeof(T));
        }

        void insert(const gBaseArray<T> &other){
            d.insertAtEnd(other.d);
        }
        void insert(const T &value){
            d.insertAtEnd(&value,1,sizeof(T));
        }

        gBaseArray<T> section(gu32 index,gu32 _size,bool newBlock=false) const{
            return gBaseArray<T>(d.section(index,_size,newBlock));
        }
        void setData(const T *array,gu32 _size,bool bCopy=true,bool bTakeDataOwnership=true){
            d.setData(array,sizeof(T),_size,bCopy,bTakeDataOwnership);
        }

        //Some well needed operators

    };

    //Some common data types to use here
    typedef gBaseArray<gu8> gByteArray;
    typedef gBaseArray<gf32> gFloatArray;
    typedef gBaseArray<gf64>gDoubleArray;
    typedef gBaseArray<gs32>gIntArray;
    typedef gBaseArray<gs16>gShortArray;
    typedef gBaseArray<gu32>gUIntArray;
    typedef gBaseArray<gu16>gUShortArray;
    //! An array of scalar numbers
    typedef gBaseArray<gScalar> gScalarArray;

}
#endif // GBASEARRAY_H
