#include "gGuillotine3D.h"
#include <iostream>
#include <limits>
#include <vector>
#include <algorithm>
#include <gArray/gDualList.h>
#include "gBpTools.h"
using namespace gfw;

gGuillotine3D::gGuillotine3D():
    m_width(0),
    m_height(0),
    m_depth(0),
    m_dimensionMinRation(0.3f),
    m_maxHeightDifferential(0),
    m_maxLenghtWithHightDifferential(0),
    m_maxHangOut(0){

}
gf32 gGuillotine3D::getScoreByHeuristics(const gBpBox &box, const gBpBox &freeBox){
    return freeBox.y + box.height;
}
bool gGuillotine3D::boxMeetsBHRatio(const gBpBox &b){
    gf32 min = MIN(b.width,b.depth);
    gf32 minallowed =b.height * m_dimensionMinRation;

    return min >=minallowed;
}

void gGuillotine3D::splitFreeBoxAlongAxis(const gBpBox &freeBox,
                                          const gBpBox &placedBox,
                                          bool splitHorizontal,
                                          bool useOriginal){
    /* Form the three new cuboids. Two of them (bottom and right) are placed on
         * the same level as placed cuboid and their height is the same as for
         * placedCuboid. The third one (top) is placed on top of bottom and right
         * cuboid and has a base area the same as freeCuboid base. */
        gf32 maxhangout = MAX(placedBox.depth,placedBox.width);
        maxhangout*=m_maxHangOut;

        gBpBox bottom;
        gf32 topy=freeBox.y + placedBox.height;
        bottom.x = freeBox.x;
        bottom.y = freeBox.y;
        bottom.z = freeBox.z + placedBox.depth;
        bottom.depth= freeBox.depth- placedBox.depth;
        if(useOriginal)
            bottom.height = placedBox.height; //changed this from original
        else
            bottom.height = m_height - bottom.y; //this is the replaceement


        gBpBox right;
        right.x = freeBox.x + placedBox.width;
        right.y = freeBox.y;
        right.z = freeBox.z;
        right.width = freeBox.width - placedBox.width;
        if(useOriginal)
            right.height = placedBox.height; //changed this from original
         else
            right.height = bottom.height; //replaced this
        gBpBox top;
        top.x = freeBox.x;
        top.y = topy;
        top.z = freeBox.z;

            if(useOriginal)
                top.height = freeBox.height - top.y; //changed this from original
            else
                top.height=m_height - top.y;

        //changed this from original
        if(useOriginal){
            top.width = freeBox.width;
            top.depth =  freeBox.depth;
        }else{
            // this is replacement

            top.width=placedBox.width;
            top.depth=placedBox.depth;
            if(placedBox.width > placedBox.depth){
                top.width+=maxhangout;
                right.x+=maxhangout;
            }
            else if(placedBox.width < placedBox.depth){
                top.depth+=maxhangout;
                bottom.z+=maxhangout;
            }
            else{
                maxhangout*=0.5f;
                top.width+=maxhangout;
                top.depth+=maxhangout;
                right.x+=maxhangout;
                bottom.z+=maxhangout;

            }
        }

       if (splitHorizontal)
        {
            bottom.width = freeBox.width;
            right.depth =  placedBox.depth;
        }
        else // Split vertically
        {
            bottom.width = placedBox.width;
            right.depth = freeBox.depth;
        }


        // Add new free gBpBoxs.
        m_freeboxes.append(bottom);//front
        m_freeboxes.append(right);//right
         //expandFreeBox(m_freeboxes.size() - 1);

     //   expandFreeBoxRF(top,top);
        //simple if can have box up then dont' add a space on top
        if(placedBox.bBoxOver)
            m_freeboxes.append(top);//bottom
}

void gGuillotine3D::splitFreeBoxByHeuristics(const gBpBox &freeBox, const gBpBox &placedBox, G_GUILLOTINE3D_SPLIT_METHODS method){
    // Compute the lengths of the leftover area.
        const gs32 w = freeBox.width - placedBox.width;
        const gs32 d = freeBox.depth- placedBox.depth;

        // Use the given heuristic to decide which choice to make.

        bool splitHorizontal;
        switch(method){
        case G_G3D_SPLIT_SHORTER_LEFTOVER_AXIS:
            // Split along the shorter leftover axis.
            splitHorizontal = (w <= d);
            break;
        case G_G3D_SPLIT_LONGER_LEFTOVER_AXIS:
            // Split along the longer leftover axis.
            splitHorizontal = (w > d);
            break;
        case G_G3D_SPLIT_SHORTER_AXIS:
            // Split along the shorter total axis.
            splitHorizontal = (freeBox.width <= freeBox.depth);
            break;
        case G_G3D_SPLIT_LONGER_AXIS:
            // Split along the longer total axis.
            splitHorizontal = (freeBox.width > freeBox.depth);
            break;
        default:
            splitHorizontal = true;
            assert(false);
        }

        // Perform the actual split.
        splitFreeBoxAlongAxis(freeBox, placedBox, splitHorizontal);
}

gBpBox gGuillotine3D::findPositionForNew(const gBpBox &box, gs32 *index, bool *bWasPlaced){
    gf32 width = box.width;
    gf32 height = box.height;
    gf32 depth = box.depth;
    gBpBox bestPosition,freebox;
    gf32 score;

    gf32 bestScore = (gf32)std::numeric_limits<float>::max();

    // Try each free box to find the best one for placement a given box.
    // Rotate a box in every possible way and find which choice is the best.
    sortFreeBoxes();//sort the free boxes placing those near the floor as closer

    for (gu32 i = 0; i < m_freeboxes.size(); ++i){
        // Width x Height x Depth
        if (width <= m_freeboxes[i].width &&
                height <= m_freeboxes[i].height &&
                depth <= m_freeboxes[i].depth)
        {
            score = getScoreByHeuristics(box, m_freeboxes[i]);
            if (score < bestScore)
            {

                if(bWasPlaced)*bWasPlaced=true;
                bestPosition.x = m_freeboxes[i].x;
                bestPosition.y = m_freeboxes[i].y;
                bestPosition.z = m_freeboxes[i].z;
                bestPosition.width=box.width;
                bestPosition.height=box.height;
                bestPosition.depth=box.depth;

                bestScore = score;
                *index = i;
            }
        }

        // Width x Depth x Height
        if (width <= m_freeboxes[i].width &&
                depth <= m_freeboxes[i].height &&
                height <= m_freeboxes[i].depth)
        {
            score = getScoreByHeuristics(box, m_freeboxes[i]);
            if (score < bestScore){
                if(bWasPlaced)*bWasPlaced=true;
                bestPosition.x = m_freeboxes[i].x;
                bestPosition.y = m_freeboxes[i].y;
                bestPosition.z = m_freeboxes[i].z;
                bestPosition.width = width;
                bestPosition.height = depth;
                bestPosition.depth = height;

                bestScore = score;
                *index = i;
            }
        }

        // Depth x Height x Width
        if (depth <= m_freeboxes[i].width &&
                height <= m_freeboxes[i].height &&
                width <= m_freeboxes[i].depth)
        {
            score = getScoreByHeuristics(box, m_freeboxes[i]);
            if (score < bestScore)
            {
                if(bWasPlaced)*bWasPlaced=true;
                bestPosition.x = m_freeboxes[i].x;
                bestPosition.y = m_freeboxes[i].y;
                bestPosition.z = m_freeboxes[i].z;
                bestPosition.width = depth;
                bestPosition.height = height;
                bestPosition.depth = width;
                bestScore = score;
                *index = i;
            }
        }

        // Depth x Width x Height
        if (depth <= m_freeboxes[i].width &&
                width <= m_freeboxes[i].height &&
                height <= m_freeboxes[i].depth)
        {
            float score = getScoreByHeuristics(box, m_freeboxes[i]);
            if (score < bestScore)
            {
                if(bWasPlaced)*bWasPlaced=true;
                bestPosition.x = m_freeboxes[i].x;
                bestPosition.y = m_freeboxes[i].y;
                bestPosition.z = m_freeboxes[i].z;
                bestPosition.width = depth;
                bestPosition.height = width;
                bestPosition.depth = height;
                bestScore = score;
                *index = i;
            }
        }

        // Height x Width x Depth
        if (height <= m_freeboxes[i].width &&
                width <= m_freeboxes[i].height &&
                depth <= m_freeboxes[i].depth)
        {
            score = getScoreByHeuristics(box, m_freeboxes[i]);
            if (score < bestScore)
            {
                if(bWasPlaced)*bWasPlaced=true;
                bestPosition.x = m_freeboxes[i].x;
                bestPosition.y = m_freeboxes[i].y;
                bestPosition.z = m_freeboxes[i].z;
                bestPosition.width = height;
                bestPosition.height = width;
                bestPosition.depth = depth;
                bestScore = score;
                *index = i;
            }
        }

        // Height x Depth x Width
        if (height <= m_freeboxes[i].width &&
                width <= m_freeboxes[i].height &&
                depth <= m_freeboxes[i].depth)
        {
            score = getScoreByHeuristics(box, m_freeboxes[i]);
            if (score < bestScore)
            {
                if(bWasPlaced)*bWasPlaced=true;
                bestPosition.x = m_freeboxes[i].x;
                bestPosition.y = m_freeboxes[i].y;
                bestPosition.z = m_freeboxes[i].z;
                bestPosition.width = height;
                bestPosition.height = depth;
                bestPosition.depth = width;
                bestScore = score;
                *index = i;
            }
        }
    }

    return bestPosition;
}
gf32 gGuillotine3D::boxFitCheck(const gBpBox &freespace,
                                const gBpBox &pbox,
                                gBpBox &out,
                                bool *bFits){
    gf32 width = pbox.width;
    gf32 height = pbox.height;
    gf32 depth = pbox.depth;
    gBpBox bestPosition,box;
    bestPosition.addon = pbox.addon;
    gf32 score;

    gf32 bestScore = (gf32)std::numeric_limits<float>::max();
    if(bFits)*bFits = false;
    if (width <= freespace.width &&
            height <= freespace.height &&
            depth <= freespace.depth)
    {
        box.setDimensions(width,height,depth);
        score = getScoreByHeuristics(box, freespace);
        if (score < bestScore)
        {

            if(bFits)*bFits=true;
            bestPosition.x = freespace.x;
            bestPosition.y = freespace.y;
            bestPosition.z = freespace.z;
            bestPosition.width=width;
            bestPosition.height=height;
            bestPosition.depth=depth;

            bestScore = score;

        }
    }

    // Width x Depth x Height
    if (width <= freespace.width &&
            depth <= freespace.height &&
            height <= freespace.depth)
    {
        box.setDimensions(width,depth,height);
        score = getScoreByHeuristics(box, freespace);
        if (score < bestScore){
            if(bFits)*bFits=true;
            bestPosition.x = freespace.x;
            bestPosition.y = freespace.y;
            bestPosition.z = freespace.z;
            bestPosition.width = width;
            bestPosition.height = depth;
            bestPosition.depth = height;

            bestScore = score;

        }
    }

    // Depth x Height x Width
    if (depth <= freespace.width &&
            height <= freespace.height &&
            width <= freespace.depth)
    {
        box.setDimensions(depth,height,width);
        score = getScoreByHeuristics(box, freespace);
        if (score < bestScore)
        {
            if(bFits)*bFits=true;
            bestPosition.x = freespace.x;
            bestPosition.y = freespace.y;
            bestPosition.z = freespace.z;
            bestPosition.width = depth;
            bestPosition.height = height;
            bestPosition.depth = width;
            bestScore = score;

        }
    }

    // Depth x Width x Height
    if (depth <= freespace.width &&
            width <= freespace.height &&
            height <= freespace.depth)
    {
        box.setDimensions(depth,width,height);
        float score = getScoreByHeuristics(box, freespace);
        if (score < bestScore)
        {
            if(bFits)*bFits=true;
            bestPosition.x = freespace.x;
            bestPosition.y = freespace.y;
            bestPosition.z = freespace.z;
            bestPosition.width = depth;
            bestPosition.height = width;
            bestPosition.depth = height;
            bestScore = score;

        }
    }

    // Height x Width x Depth
    if (height <= freespace.width &&
            width <= freespace.height &&
            depth <= freespace.depth)
    {
        box.setDimensions(height,width,depth);
        score = getScoreByHeuristics(box, freespace);
        if (score < bestScore)
        {
            if(bFits)*bFits=true;
            bestPosition.x = freespace.x;
            bestPosition.y = freespace.y;
            bestPosition.z = freespace.z;
            bestPosition.width = height;
            bestPosition.height = width;
            bestPosition.depth = depth;
            bestScore = score;

        }
    }

    // Height x Depth x Width
    if (height <= freespace.width &&
            depth <= freespace.height &&
            width <= freespace.depth)
    {
        box.setDimensions(height,depth,width);
        score = getScoreByHeuristics(box, freespace);
        if (score < bestScore)
        {
            if(bFits)*bFits=true;
            bestPosition.x = freespace.x;
            bestPosition.y = freespace.y;
            bestPosition.z = freespace.z;
            bestPosition.width = height;
            bestPosition.height = depth;
            bestPosition.depth = width;
            bestScore = score;

        }
    }

    if(*bFits && boxMeetsBHRatio(box)){
        if((bestPosition.y + bestPosition.height) > m_height){
            //exceds bin height
            *bFits=false;
            return 0;
        }
        out = bestPosition;
        return gMath::squareRoot(out.width * out.width + out.depth * out.depth);
    }
    return 0;
}

gBpBox *gGuillotine3D::insertBox(const gBpBox &box, G_GUILLOTINE3D_SPLIT_METHODS method){
    gs32 freeNodeIndex = 0;
    bool wasPlaced=false;

    gBpBox newPosition = findPositionForNew(box,&freeNodeIndex,&wasPlaced);
    // Abort if we didn't have enough space in the bin
    if (!wasPlaced)
        return 0;

    gBpBox *newBox=new gBpBox(newPosition);



    // Remove the space that was just consumed by the new cuboid
    splitFreeBoxByHeuristics(m_freeboxes[freeNodeIndex], *newBox, method);
    m_freeboxes.remove(freeNodeIndex,1);

    // Remember the new used cuboid
    m_boxes.append(newBox);

    return newBox;
}

gList<gBpBox> gGuillotine3D::insertBoxList(gList<gBpBox> &boxes,gList<gBpBox> &nonPlaced, G_GUILLOTINE3D_SPLIT_METHODS method){
    gList<gBpBox> foundPlaces;
    foundPlaces.setRemoveReferences(false);
    nonPlaced.setRemoveReferences(false);
    for (gu32 i=0;i<boxes.size();i++){
        gBpBox *box = boxes.pValue(i);
        gBpBox *place = insertBox(*box, method);
        if (place){
            box->x = place->x + (0.5 * place->width);
            box->z = place->z + (0.5 * place->depth);
            box->y = place->y + 0.5 * place->height;
            box->width = place->width;
            box->height = place->height;
            box->depth = place->depth;
            foundPlaces.append(box);
        }
        else
        {
            std::cout << "Place not found!:" << std::endl;
            std::cout << "Width: " << box->width << std::endl;
            std::cout << "Height: " << box->depth<< std::endl;
            nonPlaced.append(box);

        }
    }
    foundPlaces.setShared(true);
    return foundPlaces;
}
gList<gBpBox> gGuillotine3D::packBoxes(gList<gBpBox> &boxes, G_GUILLOTINE3D_SPLIT_METHODS method, const gBpBox *smallest){
    gBpBox *freeBox,*box,fitbox,minfitbox;
    const gBpBox *minbox;
    gList<gBpBox> outcontainer;
    gList<gBpBox> temp;
    gu32 minindex,lastsize,i = 0,pases=0;
    //value to big to be real for a package
    gf32 fval,fsval;
    if(!smallest)
        minbox = (const gBpBox *)gBpTools::minBox(boxes);
    else
        minbox = smallest;

    bool done = false,fits;
    temp.copyFromOther(boxes);

    while(!done){


        freeBox = m_freeboxes.pValue(i);
        //freeBox = expandFreeBox(i);
        minindex = gu32(-1);
        fval = 0;
        // iterate all boxes in search for the best fit for the free space
        for(gu32 j=0;j<temp.size();j++){
            box = temp.pValue(j);
            fsval = boxFitCheck(*freeBox,*box,fitbox,&fits);
            // best fit is the box that has less space left
            if(fsval > fval && fits){
                fval = fsval;
                minindex = j;
                minfitbox = fitbox;
            }

        }
        // we have the a box that fits
        if(minindex != gu32(-1)){

            // remove box from our working list
            temp.remove(minindex,1);
            // split the free space
            splitFreeBoxByHeuristics(*freeBox,minfitbox,method);
            // remove the free space reference
            m_freeboxes.remove(i,1);
            //we have a match

            //minfitbox.steps = m_freeboxes;
            outcontainer.append(minfitbox);
            m_boxes.append(minfitbox);
            closeGaps(minbox);
            sortFreeBoxes(); // sort our free space to position lower places to be proceced first
            m_boxes[m_boxes.size() -1].steps = m_freeboxes;

        }
        // if container is full then we could fit all boxes
        if(outcontainer.size() == boxes.size()){
            done = true;
            continue;
        }

        if(lastsize!=m_freeboxes.size()){ // size of the free space increased then
            i=0; //we reset the index to 0 to start from 0
            //sortFreeBoxes(); // sort our free space to position lower places to be proceced first
            pases =0; //
        }else{ // size did not changed means no box could be found for space then change the counter
           i++;
        }
        lastsize = m_freeboxes.size(); // set our last iteration size
        if(i == lastsize) // completed to check all remaining freespaces we are done
            done = true;

    }
    outcontainer.setShared(true);
    return outcontainer;
}


void gGuillotine3D::sortFreeBoxes(){
   gBpTools::sortBoxes(m_freeboxes, GBP_SORT_BYMINHEIGHTPOSITION);
}

struct _bc{
    gf32 *_t;
    gf32 *_u;
    gf32 *_m;
    gf32 *_n;
    gf32 *_h;
    gf32 *_y;

    void setup(gBpBox *box, gs32 mode){
        switch(mode){
        case G_BP_COLLISION_FRONT:// u=  depth t=width, x=m,z=n
            _u = &box->depth;_t = &box->width;
            _m = &box->x; _n = &box->z;
            break;
        case G_BP_COLLISION_RIGHT:// u = width  t=depth , x=n,z=m
            _u = &box->width;_t = &box->depth;
            _m = &box->z; _n = &box->x;
            break;
        default:break;
        }
        _h = &box->height;
        _y = &box->y;
    }
    gf32 &t(){return *_t;}
    gf32 &u(){return *_u;}
    gf32 &m(){return *_m;}
    gf32 &n(){return *_n;}
    gf32 &h(){return *_h;}
    gf32 &y(){return *_y;}
    gf32 area(){return *_t * (*_u);}
};

bool gGuillotine3D::expandFreeBox(gu32 index,const gBpBox *smallest){
    gBpBox &box = m_freeboxes.value(index);
    gBpBox *ret,r1,r2,r3,c;
    gu32 collisionIndex;
    bool cond = false;
    _bc A,B,R1,R2,R3,C;
    bool bfirstattempt = false;
    gs32 _mode;



    r1.isResidue = 1;
    r2.isResidue = 1;
    r3.isResidue = 1;
    //Check requirements
    //if the box can't hold a box then check if there is a chance expanding it would help to hold one
 /*   if(box.width< smallest->width && box.depth < smallest->depth){
        // cant' hold any box it would be stuipid to even try expanding this box
        return false;
    }
    if(box.width < smallest->width || box.depth < smallest->depth){
        //at leat one of the sides can hold a side of the box then check it which
        if(box.width >= smallest->width)
            _mode = G_BP_COLLISION_BACK;
        else
            _mode = G_BP_COLLISION_LEFT;
        //Check if the biggest are have a box next to it
        if(gBpTools::boxCollides(box,m_boxes,(G_BP_COLLISION)_mode,0,&ret,&collisionIndex)){
            //there is a box behind the free space, there is a chance expanding it may help to increase
            //the box to hold a box
            if(_mode == G_BP_COLLISION_LEFT) _mode = G_BP_COLLISION_RIGHT;
            else _mode = G_BP_COLLISION_FRONT;

        }else{
            //box to damn small on a side
            if(_mode == G_BP_COLLISION_LEFT) _mode = G_BP_COLLISION_FRONT;
            else _mode = G_BP_COLLISION_RIGHT;
        }
    }else{

        if(box.width >= box.depth) _mode = G_BP_COLLISION_FRONT;
        else if(box.depth>=box.width)_mode=G_BP_COLLISION_RIGHT;
    }*/
    _mode = G_BP_COLLISION_FRONT;
    if(gBpTools::boxCollides(box,m_freeboxes,(G_BP_COLLISION)_mode,0.05f,true,&ret,&collisionIndex)){
        bfirstattempt = true;
    }
    if(!bfirstattempt){
        //try other way around
        if(_mode == G_BP_COLLISION_FRONT)
            _mode = G_BP_COLLISION_RIGHT;
        else if(_mode == G_BP_COLLISION_RIGHT)
            _mode = G_BP_COLLISION_FRONT;
        if(gBpTools::boxCollides(box,m_freeboxes,(G_BP_COLLISION)_mode,0.05f,true,&ret,&collisionIndex)){
            bfirstattempt = true;
        }
    }

    //verify whether we are at the same altitud
    if(!bfirstattempt) return false;
    A.setup(&box,_mode);
    B.setup(ret,_mode);
    R1.setup(&r1,_mode);
    R2.setup(&r2,_mode);
    R3.setup(&r3,_mode);
    C.setup(&c,_mode);
    bool bAllow;
    gf32 mhd = 0;
    if(m_maxHeightDifferential > 0){
        if(B.h() >= A.h())
            mhd = B.h() - A.h();
        else
            mhd = A.h() - B.h();
        if(mhd > 0){
            gf32 mhy;
            gf32 yval;
            gf32 dval;
            if(B.h() > A.h()){
                mhy = A.y() - mhd;
                yval = B.y();dval =B.u();
            }
            else{
                mhy = B.y() - mhd;
                yval = A.y();dval = A.u();
            }
            gf32 topAy=A.h() + A.y();
            gf32 topBy=B.h() + B.y();

            bAllow = (mhd <= m_maxHeightDifferential) &&
                    (mhy == yval) &&
                    (topAy == topBy) &&
                    dval <= m_maxLenghtWithHightDifferential;


        }else{
            bAllow = A.h() == B.h() && A.y() == B.y();
        }
    }else{
        if(A.y() != B.y() && B.h() == A.h())
            mhd = 0;
        bAllow = A.h() == B.h() && A.y() == B.y();
    }
    if(bAllow)
    {
        //either width is smaller than depth or didn't meet criteria we try again
        c = box; //make sure we copy everything
        c.isResidue = 2;
        // r3 is the residue when chopin the box from bottom when y is lower of our box

        gf32 area1,area2;
        if(A.t() <= B.t()){
            area1 = A.t() * (A.u() + B.u());
            area2 = B.area();
        }
        else{
            area1 = B.t() * (A.u() + B.u());
            area2 = A.area();
        }
        C.u() = A.u() + B.u();
        C.n() = A.n();

        if(area1 > area2){
            //meet criteria
            if((A.t() <= B.t()) && (A.m()>=B.m()) && ((A.m()+A.t()) <= (B.m()+B.t()))){
                C.t() = A.t(); C.m() = A.m();
                const gf32 dm =A.m() - B.m();
                R1.u() = B.u(); R1.t() = dm;
                R1.m() = B.m(); R1.n() = B.n();
                R2.u() = B.u(); R2.t() = B.t() - (dm + A.t());
                R2.m() = A.m() + A.t(); R2.n() = B.n();
                R1.h() = R2.h() = B.h();
                R1.y() = R2.y() = R3.y() = B.y();
                R3.u() = B.u();
                R3.t() = B.t();
                R3.m() = B.m();
                R3.n() = B.n();
                R3.h() = mhd;


                cond = true;

            } else if((B.t() <= A.t()) && (B.m()>=A.m()) && ((B.m()+B.t()) <= (A.m()+A.t()))){
                C.t() = B.t(); C.m() = B.m();
                const gf32 dm =B.m() - A.m();
                R1.u() = A.u(); R1.t() = dm;
                R1.m() = A.m(); R1.n() = A.n();
                R2.u() = A.u(); R2.t() = A.t() - (dm + B.t());
                R2.m() = B.m() + B.t(); R2.n() = A.n();
                R1.h() = R2.h() = A.h();
                R1.y() = R2.y() = R3.y() = A.y();
                R3.u() = A.u();
                R3.t() = A.t();
                R3.m() = A.m();
                R3.n() = A.n();
                R3.h() = mhd;
                cond = true;
            }
            if(cond){
                // Check the residual boxes and make sure their sizes are not big enough to cause horrible gaps between boxes

                m_freeboxes.remove(index,1);
                m_freeboxes.remove(ret,1);
                m_freeboxes.append(c);
                if(R1.area() > 0){
                    m_freeboxes.append(r1);
                }
                if(R2.area() > 0){
                    m_freeboxes.append(r2);
                }
                if(mhd!=0){

                    m_freeboxes.append(r3);
                }
            }
        }
    }


    return cond;
}

bool gGuillotine3D::expandFreeBoxRF(const gBpBox &freeBox,gBpBox &suggestedSize){
    //function tries to expand a box on right on front depending on if there are no more boxes ahead and if there is
    //a box under it
    //first raycast to the right
    gf32 rayResult = m_width;
    gBpBox *foundBoxRight,*foundBoxBottom,*foundBoxFront;

    bool cantexpandright = gBpTools::boxRayCast(freeBox,m_boxes,G_BP_COLLISION_RIGHT,rayResult,&foundBoxRight);
    rayResult = m_depth;
    bool cantexpandfront = gBpTools::boxRayCast(freeBox,m_boxes,G_BP_COLLISION_FRONT,rayResult,&foundBoxFront);

    //why stop here because if there are boxes at this point means there are free spaces on the way.
    if(cantexpandright && cantexpandfront) return false;

    //tricky part..need to search boxes suggesting a virtual box of limited size that moves towards both sides
    gBpBox vb = freeBox;

    //try on right

    for(vb.x = freeBox.x; vb.x <=m_width; vb.x+=0.1f ){
        rayResult = m_height;
        bool result = gBpTools::boxRayCast(vb,m_boxes,G_BP_COLLISION_BOTTOM,rayResult,&foundBoxBottom);
        if(result && rayResult == 0){
            for(vb.z = freeBox.z; vb.z <=m_depth; vb.z+=0.1f ){
                rayResult = m_height;
                bool result = gBpTools::boxRayCast(vb,m_boxes,G_BP_COLLISION_BOTTOM,rayResult,&foundBoxBottom);
                if(result==0 || rayResult != 0){
                    break; //nothing to do
                }

            }
        }else{
            break; //nothing to do
        }

    }

    if(vb == freeBox)
        return false; // no changes

   suggestedSize.width+=vb.x + vb.width;
   suggestedSize.depth+=vb.z + vb.depth;

   return true;


}

void gGuillotine3D::closeGaps(const gBpBox *minbox){
    gu32 i = 0;
    bool val;
    while(i < m_freeboxes.size()){
        val = expandFreeBox(i,minbox);
        if(!val){
            i++;
        }else{
            i = 0;
        }
    }
}

void gGuillotine3D::clear(){
    m_freeboxes.clear();
    m_boxes.clear();

}
void gGuillotine3D::reset(){
    clear();
    m_freeboxes.append(new gBpBox(m_width,m_height,m_depth));
}
