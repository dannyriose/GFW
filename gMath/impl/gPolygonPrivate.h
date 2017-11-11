#ifndef GPOLYGONPRIVATE_H
#define GPOLYGONPRIVATE_H
#include <gMath/gPolygon.h>
#include <float.h>


namespace gfw{
struct gPolygonPrivate:public gSharedObject{
    //! Points
    gList<gVector2f> points;
    //! Area
    gScalar area;
    //! Perimenter
    gScalar perimeter;
    //! Centroid
    gVector2f centroid;
    //! Radius
    gScalar radius;
    //! Reference vector
    gVector2f ref;
    //! set flag has changed
    void setChanged(){
        area=0;
        perimeter=0;

    }

    void calcArea(){
        gu32 last=points.size() - 1;
        area=0;
        gVector2f *v1,*v2;
        for(gu32 i=0;i<points.size();i++){
            v1=points.pValue(last);
            v2=points.pValue(i);
            area+=(v1->x + v2->x) * (v1->y - v2->y);
            last=i;
        }
        area= area * 0.5f; // /2
    }

    void calcPerimeter(){
        gu32 last=points.size() - 1;
        perimeter=0;
        gVector2f *v1,*v2;
        for(gu32 i=0;i<points.size();i++){
            v1=points.pValue(last);
            v2=points.pValue(i);
            perimeter+=v1->distanceFrom(*v2);
            last=i;
        }
    }

    void calcCentroidAreaPerimeter(){
        gu32 last=points.size() - 1;
        gScalar term,sixarearec;
        centroid.set(0,0);
        area=0;
        perimeter=0;
        gVector2f *v1,*v2;
        for(gu32 i=0;i<points.size();i++){
            v1=points.pValue(last);
            v2=points.pValue(i);
            // This term is used for x and y so calculate it and store it in a variable
            term=(v1->x * v2->y -v2->x * v1->y);
            centroid.x=(v1->x + v2->x) * term;
            centroid.y=(v1->y + v2->y) * term;
            area+=(v1->x + v2->x) * (v1->y - v2->y);
            perimeter+=v1->distanceFrom(*v2);
            last=i;
        }
        area*=0.5f;// Divide between 2..multiplication is faster.
        // Get reciprocal term 1/6A
        sixarearec=1.0f/(area * 6.0f);
        centroid.x*=sixarearec;
        centroid.y*=sixarearec;

    }
    void calcRadius(){
        //calculate the radius
        gVector2f *v;
        radius=0;
        gScalar dist,themax=0,themin=FLT_MAX;
        for(gu32 i=0;i<points.size();i++){
            v=points.pValue(i);
            dist=v->distanceFrom(centroid);
            themax = MAX(themax,dist);
            themin = MIN(themin,dist);
        }
        radius=(themax + themin) * 0.5f; //use a mean
    }

    gPolygonPrivate():area(0),
        perimeter(0),
        radius(0){

    }
    ~gPolygonPrivate(){

    }
    virtual void copy(const gSharedObject *other){
        const gPolygonPrivate *o=static_cast<const gPolygonPrivate *>(other);
        points.copyFromOther(o->points);
        area=o->area;
        perimeter=o->perimeter;
        centroid=o->centroid;
        radius=o->radius;
    }
};
G_SHAREDOBJECT_FACTORY(gPolygonPrivateFactory,gPolygonPrivate);
}
#endif // GPOLYGONPRIVATE_H
