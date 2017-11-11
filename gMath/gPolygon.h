#ifndef GPOLYGON3_H
#define GPOLYGON3_H

#include <gfw_global.h>
#include "gMath.h"
#include <gArray/gList.h>
#include "gVector2.h"
namespace gfw{
    //! Utility class that represents a simple Polygon
    class SHARED_GFW gPolygon:public gBaseShared{

    public:
        //! Constructor
        gPolygon();
        //! Copy constructor
        gPolygon(const gPolygon &other);
        //! Destructor
        virtual ~gPolygon();
        //!Adds a point to the polygon
        void addPoint(gScalar x,gScalar y);
        //! Addss a point to the polygon overloaded
        void addPoint(const gVector2f &coord);
        //! Gets a point from the polygon
        gVector2f &getPoint(gu32 index);
        //! Gets a point from the polygon const
        const gVector2f &getPoint(gu32 index) const;
        //! Reserves space for the point list
        bool alloc(gu32 size);
        //! Sets a point given an index
        void setPoint(gScalar x,gScalar y, gu32 index);
        //! Sets a point given and vector
        void setPoint(const gVector2f &v, gu32 index);
        //! Gets the total count of points
        gu32 pointCount() const;
        //! Remove Point
        void removePoint(gu32 index);
        //! Removes a point given a coordinate
        void removePoint(gScalar x,gScalar y);
        //! Overloaded remove point give a vector coordinate
        void removePoint(const gVector2f &coord);
        //! Checks whether a point exists on space
        bool hasPoint(gScalar x,gScalar y,gu32 *indexOut=0) const;
        //! Checks whether a point exists on space
        bool hasPoint(const gVector2f &coord,gu32 *indexOut=0) const;
        //! Verified if the point is inside the polygon or touches it
        bool isPointInArea(const gVector2f &point) const;
        //! Verifies if the point is inside the polygon or touches it.
        bool isPointInArea(gScalar x, gScalar y) const;
        //! Gets the area of the polygon
        gScalar area() const;
        //! Gets the perimeter of the polygon
        gScalar perimeter() const;
        //! Gets the centroid of the polygon
        const gVector2f &centroid() const;
        //! Gets the radius of the polygon
        gScalar radius() const;
        //! Line Count
        gu32 lineCount() const;
        //! Calculates all referenced to the polygon: area,perimeter,centroid,radius
        void calculate();
        //! Gets whether the object is empty or does not have any point
        virtual bool isEmpty() const;
        //! Operator =
        gPolygon &operator ==(const gPolygon &other);
        //! Gets a point operator []
        gVector2f &operator [] (int index);
        //! Gets a point given and index
        const gVector2f &operator [] (int index) const;
        //! Creates a polygon object by rectangular dimensions
        static gPolygon fromRectangularDimensions(const gVector2f &dimensions,const gVector2f &offset=gVector2f());

    };
}


#endif // GPOLYGON3_H
