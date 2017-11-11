#ifndef GGUILLOTINE3D_H
#define GGUILLOTINE3D_H

/*********************************************************************************************/



#include "gBpBox.h"
#include <gMath/gVector3.h>
namespace gfw{
    //! Implements the guillotine 3D algorithm for bin packing
    /** This object not need to be declared as shared */
    enum G_GUILLOTINE3D_SPLIT_METHODS{
        G_G3D_SPLIT_SHORTER_LEFTOVER_AXIS,
        G_G3D_SPLIT_LONGER_LEFTOVER_AXIS,
        G_G3D_SPLIT_SHORTER_AXIS,
        G_G3D_SPLIT_LONGER_AXIS
    };

    enum G_GUITLLOTINE3D_INDEX{
        G_G3D_TOP_BOX,
        G_G3D_BOTTOM_BOX,
        G_G3D_LEFT_BOX,
        G_G3D_RIGHT_BOX,
        G_G3D_FRONT_BOX,
        G_G3D_REAR_BOX,
        G_G3D_WEIGHT
    };

    //! Implements an efficient bin packing algorithm using the guillotine 3D algorithm
    class SHARED_GFW gGuillotine3D{
    protected:
        //! The list of boxes contained in this bin
        gList<gBpBox> m_boxes;
        //! List of free positions for boxes
        gList<gBpBox> m_freeboxes;
        //! The bin dimensions
        gf32 m_width;
        //! Bin height
        gf32 m_height;
        //! Bin depth
        gf32 m_depth;
        //! Additional information
        gVariantList m_addon;
        //! Dimension ratio.
        /** Defines of the size of the base of the box to be placed
         * a .5 the minumum allowed width or depth is if is the same size of height.
         * meaning that if .3 is speciified then the minimum base to height ratio should be the
         * 30% of height.
         * default is 0.3 */
        gf32 m_dimensionMinRation;
        //! Maximum high differential allowed when expanding addiacent free spaces.
        /** This member determines the maximum high differential when expanding to free spaces
         * */
        gf32 m_maxHeightDifferential;
        //! Maximum lenght allowed when expanding addiacent free spaces with height differential bigger than 0
        gf32 m_maxLenghtWithHightDifferential;
        //! Maximum hangout of boxes on top. Default 0. This is a percent value.
        gf32 m_maxHangOut;

    public:
        //!Class constructor
        gGuillotine3D();
        //! Virtual destructor in case of subclassing
        virtual ~gGuillotine3D(){

        }
        //! Create the bin
        void create(gf32 width,gf32 height,gf32 depth){
            m_width=width;m_height=height;m_depth=depth;
            m_freeboxes.append(new gBpBox(width,height,depth));

        }
        //! Insert add on information
        void addInfo(const gVariant &info){
            m_addon.append(info);
        }

        //! Get information given an index
        const gVariant &getInfo(gu32 index) const{
            return m_addon.value(index);
        }
        //! Gets the count additional data
        gu32 getInfoCount() const{
            return m_addon.size();
        }
        //! Gets the width of the bin
        gf32 getWidth() const {return m_width;}
        //! Gets the height of the bin
        gf32 getHeight() const {return m_height;}
        //! Gets the depth of the bin
        gf32 getDepth() const {return m_depth;}

        //!Gets the best score to position a box by heuristics search
        gf32 getScoreByHeuristics(const gBpBox &box, const gBpBox &freeBox);
        //! Splits the free spaces boxes along an axis
        void splitFreeBoxAlongAxis(const gBpBox &freeBox,const gBpBox &placedBox,bool splitHorizontal,bool useOriginal=false);
        //! Splits the free space boxes using heuristics
        void splitFreeBoxByHeuristics(const gBpBox &freeBox,const gBpBox &placedBox,G_GUILLOTINE3D_SPLIT_METHODS method);
        //! Finds a position for a new box on any of the free space boxes
        gBpBox findPositionForNew(const gBpBox &box, gs32 *index, bool *bWasPlaced);
        //! Verifies if a box fits a free space and retunrs the space left on the free space */
        /** \returns vector dimension with free space */
        gf32 boxFitCheck(const gBpBox &freespace, const gBpBox &srcBox, gBpBox &out, bool *bFits);


        //! Adds a box to the bin
        gBpBox *insertBox(const gBpBox &box, G_GUILLOTINE3D_SPLIT_METHODS method);
        //! Adds an entire set of boxes to the list and returns the list of placed boxes
        gList<gBpBox> insertBoxList(gList<gBpBox> &boxes,gList<gBpBox> &nonplaced, G_GUILLOTINE3D_SPLIT_METHODS method);
        //! Packs a set of boxes
        gList<gBpBox> packBoxes(gList<gBpBox> &boxes, G_GUILLOTINE3D_SPLIT_METHODS method, const gBpBox *smallest = 0);
        //! Compares the max edge of a box
        static bool compareMaxEdge(const gBpBox &s1,const gBpBox &s2){
            gf32 i_max = MAX(s1.width,s1.height);i_max=MAX(i_max,s1.depth);
            gf32 j_max = MAX(s2.width,s2.height);j_max=MAX(j_max,s2.depth);
            return i_max > j_max;
        }


        //! Sort free boxes
        void sortFreeBoxes();
        //! Get free boxes list pointer
        gList<gBpBox> &getFreeBoxes(){return m_freeboxes;}
        //! Expands a free box by searching neighbor free boxes this increasing free space in the bin
        bool expandFreeBox(gu32 index, const gBpBox *minbox);
        //! Expands a free box raycasting for bottom boxes and checking right and front special for the top free box
        bool expandFreeBoxRF(const gBpBox &freeBox, gBpBox &suggestedSize);
        //! Tries to close and to increase free space between free spaces
        void closeGaps(const gBpBox *minbox);

        //! Clears the bin and all its internal boxes
        void clear();
        void reset();

        gList<gBpBox> &getBoxes(){return m_boxes;}

        bool boxMeetsBHRatio(const gBpBox &b);

        void setMinRatio(gf32 minRatio){m_dimensionMinRation = minRatio;}
        void setMaxHeightDifferential(gf32 max){m_maxHeightDifferential = max;}
        void setMaxLenghtWidthDifferential(gf32 max){m_maxLenghtWithHightDifferential = max;}
        void setMaxHangOut(gf32 value){m_maxHangOut =value;}


    };
}
#endif // GGUILLOTINE3D_H
