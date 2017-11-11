
#include "impl/gPolygonPrivate.h"
using namespace gfw;
#define G_POLYGON_PCAST(var,obj) gPolygonPrivate *var=(gPolygonPrivate *)obj
#define G_POLYGON_CREATE(var,target) gPolygonPrivate *var=new  gPolygonPrivate;target=var
#define ca G_POLYGON_PCAST(o,d)
#define cn G_POLYGON_PCAST(o,d);if(!o){o=new gPolygonPrivate;d=o;}
#define acn gassert(d,"gPolygon Object is Empty");


gPolygon::gPolygon():gBaseShared(new gPolygonPrivateFactory){

}

gPolygon::gPolygon(const gPolygon &other):gBaseShared(other,new gPolygonPrivateFactory){


}
gPolygon::~gPolygon(){
}


gPolygon &gPolygon::operator == (const gPolygon &other){
    assign(other);
    return *this;
}

bool gPolygon::alloc(gu32 size){
    clear();
    G_POLYGON_CREATE(o,d);
    bool bRet = o->points.alloc(size);
    if(bRet){
        o->points.fill();
    }
    return bRet;
}

void gPolygon::addPoint(const gVector2f &coord){
    cn;
    o->points.append(coord);

}
void gPolygon::addPoint(gScalar x, gScalar y){
    addPoint(gVector2f(x,y));
}
void gPolygon::setPoint(const gVector2f &v, gu32 index){
    cn;
    gassert(index<o->points.size(),"gPolygon index out of boudaries");
    o->points.setValue(v,index);
}
void gPolygon::setPoint(gScalar x, gScalar y, gu32 index){
    setPoint(gVector2f(x,y),index);
}
gVector2f &gPolygon::getPoint(gu32 index){
    acn;
    ca;
    return o->points[index];
}
const gVector2f &gPolygon::getPoint(gu32 index) const{
    acn;
    ca;
    return o->points[index];
}
void gPolygon::removePoint(gu32 index){
    acn;
    ca;
    o->points.remove(index,1);
}
void gPolygon::removePoint(const gVector2f &coord){
    acn;
    ca;
    gu32 index;
    if(o->points.contains(coord,&index)){
        o->points.remove(index,1);
    }
}
void gPolygon::removePoint(gScalar x, gScalar y){
    removePoint(gVector2f(x,y));
}
gu32 gPolygon::pointCount() const{
    if(!d)return 0;
    ca;
    return o->points.size();
}
bool gPolygon::hasPoint(const gVector2f &coord, gu32 *indexOut) const{
    acn;
    ca;
    return o->points.contains(coord,indexOut);
}
bool gPolygon::hasPoint(gScalar x, gScalar y, gu32 *indexOut) const{
    return hasPoint(gVector2f(x,y),indexOut);
}
bool gPolygon::isPointInArea(const gVector2f &point) const{
    acn;
    ca;
    bool  c=false;
    gu32 j=o->points.size() - 1;
    gVector2f *v1,*v2;
    gf32 term;
    //(poly[j][0] - poly[i][0]) * (y - poly[i][1]) / (poly[j][1] - poly[i][1]) + poly[i][0]
    for(gu32 i=0;i<o->points.size();i++){
        v1=o->points.pValue(j);
        v2=o->points.pValue(i);
        term=(v1->x - v2->x) * (point.y - v2->y) / (v1->y - v2->y ) +  v2->x;
        if(((v2->y > point.y)!=(v1->y > point.y)) && point.x < term){
            c=!c;
        }

        j=i;
    }
    return c;
}
bool gPolygon::isPointInArea(gScalar x, gScalar y) const{
    return isPointInArea(gVector2f(x,y));
}
gVector2f &gPolygon::operator [] (int index){
    acn;
    ca;
    return o->points[index];
}
const gVector2f &gPolygon::operator [] (int index) const{
    acn;
    ca;
    return o->points[index];
}
gScalar gPolygon::area() const{
    acn;
    ca;

    return o->area;
}
gScalar gPolygon::perimeter() const{
    acn;
    ca;

    return o->perimeter;
}
const gVector2f &gPolygon::centroid() const{
    acn;
    ca;

    return o->centroid;
}
gf32 gPolygon::radius() const{
    acn;
    ca;
    return o->radius;
}

void gPolygon::calculate(){
    acn;
    ca;

    //calculate the centroid,area, and perimeter
    o->calcCentroidAreaPerimeter();
    //caluclate the radius
    o->calcRadius();
}
gu32 gPolygon::lineCount() const{
    acn;
    ca;
    return (gu32)o->points.size() * 0.5f;
}
bool gPolygon::isEmpty() const{
    if(!d)return true;
    ca;
    return o->points.isEmpty();
}
gPolygon gPolygon::fromRectangularDimensions(const gVector2f &dimensions, const gVector2f &offset){
    gPolygon p;

    p.alloc(4);
    p.setPoint(gVector2f(0,0) + offset,0);
    p.setPoint(gVector2f(0,dimensions.y) + offset,1);
    p.setPoint(dimensions + offset,2);
    p.setPoint(gVector2f(dimensions.x,0) + offset,3);
    p.calculate();
    p.setShared(true);
    return gPolygon(p);
}
