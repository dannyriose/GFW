#ifndef GBPTOOLS_H
#define GBPTOOLS_H
#include "gBpBox.h"

namespace gfw{
    enum GBP_BOXSORT_MODE{
        GBP_SORT_BYMINHEIGHTPOSITION,
        GBP_SORT_BYMAXVOLUME,
        GBP_SORT_BYMINVOLUME,
        GBP_SORT_BYMAXWIDTHDEPTH,
        GBP_SORT_BYMINWIDTHDEPTHHEIGHT,
        GBP_SORT_BYMINPOSITION,
        GBP_SORT_CLOSESTTOORIGIN
    };
    enum GBP_BOXROTATION_MODE{
        GBP_BR_WDMAX, //make sur width and depth gets the bigger value
    };
    enum G_BP_COLLISION{
        G_BP_COLLISION_RIGHT,
        G_BP_COLLISION_FRONT,
        G_BP_COLLISION_TOP,
        G_BP_COLLISION_BOTTOM,
        G_BP_COLLISION_LEFT,
        G_BP_COLLISION_BACK

    };

    class SHARED_GFW gBpTools{
    public:
        //! Compares two boxes by its volume
        static bool compareMaxVolume(const gBpBox &sl,const gBpBox &s2){
            return sl.volume() > s2.volume();
        }
        //! Compares two boxes by its min Y position in the bin
        static bool compareMinHeightPosition(const gBpBox &s1, const gBpBox &s2){
            return s1.y < s2.y;
        }
        static bool compareMinVolume(const gBpBox &s1,const gBpBox &s2){
            return s1.volume() < s2.volume();
        }
        static bool compareMaxWidthDepth(const gBpBox &s1,const gBpBox &s2){
            gScalar size1 = gMath::squareRoot(s1.width * s1.width + s1.depth * s1.depth);
            gScalar size2 = gMath::squareRoot(s2.width * s2.width + s2.depth * s2.depth);
            //return s1.width > s2.width && s1.depth > s2.depth;
            return size1 > size2;
        }
        static bool compareMinWidthDepthHeight(const gBpBox &s1,const gBpBox &s2){
            return s1.width < s2.width && s1.height < s2.height && s1.depth < s2.depth;
        }
        static bool compareMinPosition(const gBpBox &s1,const gBpBox &s2){
            gScalar dist1 = gMath::squareRoot(s1.x * s1.x + s1.y * s1.y + s1.z * s1.z);
            gScalar dist2 = gMath::squareRoot(s2.x * s2.x + s2.y * s2.y + s2.z * s2.z);

            return dist1 < dist2;
        }
        static bool compareClosest(const gBpBox &s1,const gBpBox &s2){
            gf32 dist1 = gMath::squareRoot(s1.x * s1.x + s1.z*s1.z);
            gf32 dist2 = gMath::squareRoot(s2.x * s2.x + s2.z*s2.z);
            return s1.y < s2.y && dist1 < dist2;
        }

        //! Sorts a box lists
        static void sortBoxes(gList<gBpBox> &boxes, GBP_BOXSORT_MODE mode);
        //! Sort a list of boxes by same dimensions
        static void sortBoxesBySameDimension(gList<gBpBox> &boxes, bool bBiggerFirst);
        //! sorts a list of boxes returning the list of boxes of same dimension
        static gList<gList<gBpBox> > sortyByDimension(gList<gBpBox> &boxes, gList<gBpBox> &unique);

        //! Rotates all boxes to an orientation
        static void rotateBoxes(gList<gBpBox> & boxes,
                                GBP_BOXROTATION_MODE mode);
        //! Gets the maximum height of the heighest box
        static gf32 getBinHeight(gList<gBpBox> &packedBin);
        //! Determines if a box colides with another given a vector
        /** \param box: The box to be tested for collision.
         *  \param collisionType: The collision type to be tested, or in which direction the collision will be tested.
         *  \param distance: The distance to project the vector to test.
         *  \param collidedWith: Result object which the projection vector collided.
         *  \param bCollidedFreeBox: returns true if the box it collided is a free box. False if collided with a packed box.
         * */
        static bool boxCollides(const gBpBox &box,
                                const gList<gBpBox> &boxList,
                                G_BP_COLLISION collisionType,
                                gf32 distance, bool bClosematch,
                                gBpBox **collidedWith,
                                gu32 *collisionItemIndex);
        //! Casts a ray from a box by the direccion provided by collision type by a maxdistance.
        /** \return true if colided with object */
        static bool boxRayCast(const gBpBox &box,
                            const gList<gBpBox> &boxList,
                            G_BP_COLLISION collisionType,
                            gf32 &inOutDistance,
                            gBpBox **collidedWith = 0,
                            gu32 *collisionItemIndex =0);
        static gBpBox *minBox(const gList<gBpBox> &boxList);
    };
}
#endif // GBPTOOLS_H
