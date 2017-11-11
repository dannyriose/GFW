#ifndef GTEMPLATES_H
#define GTEMPLATES_H
namespace gfw{
template <class T, class V>
struct gPair{
    T t1;
    V t2;
    gPair(){

    }
    gPair(const T &_t1,const V &_t2):t1(_t1),t2(_t2){

    }
};



}
#endif // GTEMPLATES_H
