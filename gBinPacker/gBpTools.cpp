#include "gBpTools.h"
#include <vector>
#include <algorithm>
using namespace gfw;

void gBpTools::sortBoxes(gList<gBpBox> &boxes,GBP_BOXSORT_MODE mode){
    std::vector<gBpBox> _vboxes;
    for(gu32 i=0;i<boxes.size();i++){
        _vboxes.push_back(boxes[i]);
    }

    switch(mode){
    case GBP_SORT_BYMINHEIGHTPOSITION:
        std::sort(_vboxes.begin(),_vboxes.end(),&gBpTools::compareMinHeightPosition);break;
    case GBP_SORT_BYMAXVOLUME:
        std::sort(_vboxes.begin(),_vboxes.end(),&gBpTools::compareMaxVolume);break;
    case GBP_SORT_BYMINVOLUME:
        std::sort(_vboxes.begin(),_vboxes.end(),&gBpTools::compareMinVolume);break;
    case GBP_SORT_BYMAXWIDTHDEPTH:
        std::sort(_vboxes.begin(),_vboxes.end(),&gBpTools::compareMaxWidthDepth);break;
    case GBP_SORT_BYMINWIDTHDEPTHHEIGHT:
        std::sort(_vboxes.begin(),_vboxes.end(),&gBpTools::compareMinWidthDepthHeight);break;
    case GBP_SORT_BYMINPOSITION:
        std::sort(_vboxes.begin(),_vboxes.end(),&gBpTools::compareMinPosition);break;
    case GBP_SORT_CLOSESTTOORIGIN:
        std::sort(_vboxes.begin(),_vboxes.end(),&gBpTools::compareClosest);break;
    default:break;
    }


    for(gu32 i=0;i<boxes.size();i++){
        boxes[i] = _vboxes[i];
    }

}

void gBpTools::sortBoxesBySameDimension(gList<gBpBox> &boxes, bool bBiggerFirst){
    gu32 lastindex;
  //first sort
    //Rotate boxes to make sure all have their maximum dimensions on width and depth
    rotateBoxes(boxes,GBP_BR_WDMAX);
    //sort by those with the max and width depth
    sortBoxes(boxes,GBP_SORT_BYMAXWIDTHDEPTH);
    //out vector
    gList<gBpBox> out,temp;
    // box
    gBpBox *b,*c;
    // dont clear memory references of items when funciton ends
    out.setRemoveReferences(false);
    temp.setRemoveReferences(false);
    //make a copy of the list to work with
    temp.copyFromOther(boxes,false);


    //add first uniques to list
    out.append(temp.pValue(0));
    while(out.size() < boxes.size()){
        lastindex = out.size() - 1;
        c = out.pValue(lastindex);

        for(gu32 i = 0; i < temp.size();i++){
            b = temp.pValue(i);
            if(c->sameDimensions(*b,0) && c!=b){
                out.append(b);

            }
        }
        for(gu32 i = lastindex; i < out.size(); i++){
            b = out.pValue(i);
            temp.remove(b,1);
        }
        if(!temp.isEmpty()){
            out.append(temp.pValue(0));
        }

    }

    boxes.copyFromOther(out,true);

}

gList<gList<gBpBox>> gBpTools::sortyByDimension(gList<gBpBox> &boxes, gList<gBpBox> &unique){
    gList<gList<gBpBox>> outf;

  //first sort
    //Rotate boxes to make sure all have their maximum dimensions on width and depth
    rotateBoxes(boxes,GBP_BR_WDMAX);
    //sort by those with the max and width depth
    //sortBoxes(boxes,GBP_SORT_BYMAXWIDTHDEPTH);
    //out vector
    gList<gBpBox> *outlist,temp;
    // box
    gBpBox *b,*c;
    // dont clear memory references of items when funciton ends
    unique.setRemoveReferences(false);
    temp.setRemoveReferences(false);
    //make a copy of the list to work with
    temp.copyFromOther(boxes,false);


    gu32 count = 0, index = 0;
    while(count < boxes.size()){
        c = temp.pValue(index);
        outlist = new gList<gBpBox>();
        outlist->setRemoveReferences(false);

        for(gu32 i = 0; i < temp.size();i++){
            b = temp.pValue(i);

            if(c->sameDimensions(*b,0) && c!=b){
                outlist->append(b);
                count++;
            }

        }
        if(!outlist->isEmpty()){
            outlist->append(c);count++;
        }


        for(gu32 i = 0; i < outlist->size(); i++){
            b = outlist->pValue(i);
            temp.remove(b,1);
        }
        if(!temp.isEmpty())
            count++;
        if(outlist->isEmpty()){
            delete outlist;
            index++;
        }
        else{
            outf.append(outlist);
            index = 0;
        }

    }

    unique.copyFromOther(temp,true);
    outf.setShared(true);
    return outf;

}

void gBpTools::rotateBoxes(gList<gBpBox> &boxes, GBP_BOXROTATION_MODE mode){
    gBpBox *b;
    gf32 w,h,d;
    for(gu32 i=0;i<boxes.size();i++){
        b = boxes.pValue(i);
        w = b->width;
        h = b->height;
        d = b->depth;
        switch(mode){
        case GBP_BR_WDMAX:
            if(h > w){
                b->width = h;b->height = w;
                b->orientation = GBP_BO_HWD;
            }else if( h > d){
                b->depth = h; b->height = d;
                b->orientation = GBP_BO_WDH;
            }
            break;
        default:break;
        }
    }
}

gf32 gBpTools::getBinHeight(gList<gBpBox> &packedBin){
    gf32 maxh = 0;
    gBpBox *b;
    for(gu32 i=0;i<packedBin.size();i++){
        b = packedBin.pValue(i);
        maxh = MAX(maxh, (b->y + b->height));
    }
    return maxh;
}

bool gBpTools::boxCollides(const gBpBox &box,
                           const gList<gBpBox> &boxList,
                           G_BP_COLLISION collisionType,
                           gf32 distance,
                           bool bClosematch,
                           gBpBox **collidedWith,
                           gu32 *collisionItemIndex){
    //test on free boxes
    gBpBox *b;
    bool cond[2];
    gf32 start;
    gf32 mt,p11[2],p12[2],p21[2],p22[2];
    gf32 diff;

    for(gu32 i=0; i< boxList.size();i++){
        b = boxList.pValue(i);
        if(*b == box) //same box
            continue;
        switch(collisionType){
        case G_BP_COLLISION_BOTTOM:

        case G_BP_COLLISION_TOP:
            mt=b->y;
            start = box.y + box.height;
            p11[0]=box.x;p12[0]=box.width + box.x;
            p21[0]=b->x;p22[0]=b->width + b->x;
            p11[1]=box.z;p12[1]=box.depth + box.z;
            p21[1]=b->z;p22[1]=b->depth + b->z;


            break;
        case G_BP_COLLISION_FRONT:
            mt=b->z;
            start = box.z + box.depth;
            p11[0]=box.x;p12[0]=box.width + box.x;
            p21[0]=b->x;p22[0]=b->width + b->x;
            p11[1]=box.y;p12[1]=box.height + box.y;
            p21[1]=b->y;p22[1]=b->height + b->y;

            break;
        case G_BP_COLLISION_BACK:
            mt=b->z + b->depth;
            start = box.z;
            p11[0]=box.x;p12[0]=box.width + box.x;
            p21[0]=b->x;p22[0]=b->width + b->x;
            p11[1]=box.y;p12[1]=box.height + box.y;
            p21[1]=b->y;p22[1]=b->height + b->y;
            break;
        case G_BP_COLLISION_RIGHT:
            mt=b->x;
            start = box.x + box.width;
            p11[0]=box.z;p12[0]=box.depth + box.z;
            p21[0]=b->z;p22[0]=b->depth + b->z;
            p11[1]=box.y;p12[1]=box.height + box.y;
            p21[1]=b->y;p22[1]=b->height + b->y;

            break;
        case G_BP_COLLISION_LEFT:
            mt = b->x + b->width;
            start = box.x;
            p11[0]=box.z;p12[0]=box.depth + box.z;
            p21[0]=b->z;p22[0]=b->depth + b->z;
            p11[1]=box.y;p12[1]=box.height + box.y;
            p21[1]=b->y;p22[1]=b->height + b->y;
            break;


        }
        diff = mt - start;

        if(diff <= distance && diff >=0.0f ){
            for(gu32 r=0;r<2;r++){

                cond[r]=(p11[r] >= p21[r]) &&  (p11[r] <= p22[r]);
                if(!bClosematch){
                    cond[r]= cond[r] || (p12[r] >=p21[r]) && (p12[r] <=p22[r]);
                }else{
                    cond[r]= cond[r] && (p12[r] >=p21[r]) && (p12[r] <=p22[r]);
                }
            }
            if(cond[0] && cond[1]){
                //we have collision
                *collidedWith = b;
                *collisionItemIndex = i;
                return true;
            }
            //test now backwards
            for(gu32 r=0;r<2;r++){
                cond[r]=(p21[r] >= p11[r]) &&  (p21[r] <= p12[r]);
                if(!bClosematch){
                    cond[r]= cond[r] || (p22[r] >=p11[r]) && (p22[r] <=p12[r]);
                }else{
                    cond[r]= cond[r] && (p22[r] >=p11[r]) && (p22[r] <=p12[r]);
                }
            }
            if(cond[0] && cond[1]){
                //we have collision
                *collidedWith = b;
                *collisionItemIndex = i;
                return true;
            }

        }

    }

    return false;
}

bool gBpTools::boxRayCast(const gBpBox &box,
                       const gList<gBpBox> &boxList,
                       G_BP_COLLISION collisionType,
                       gf32 &inOutDistance,
                       gBpBox **collidedWith,
                       gu32 *collisionItemIndex){
    //test on free boxes
    gBpBox *b;
    bool cond[2];
    const gf32 increment = 0.1f;
    gf32 start,end;
    gf32 mt,p11[2],p12[2],p21[2],p22[2];
    gf32 diff,dist = inOutDistance;

    for(gu32 i=0; i< boxList.size();i++){
        b = boxList.pValue(i);
        if(*b == box) //same box
            continue;
        switch(collisionType){
        case G_BP_COLLISION_BOTTOM:
            dist=-dist;
            mt=b->y + b->height;
            start = box.y;
            p11[0]=box.x;p12[0]=box.width + box.x;
            p21[0]=b->x;p22[0]=b->width + b->x;
            p11[1]=box.z;p12[1]=box.depth + box.z;
            p21[1]=b->z;p22[1]=b->depth + b->z;
            break;
        case G_BP_COLLISION_TOP:
            mt=b->y;
            start = box.y + box.height;
            p11[0]=box.x;p12[0]=box.width + box.x;
            p21[0]=b->x;p22[0]=b->width + b->x;
            p11[1]=box.z;p12[1]=box.depth + box.z;
            p21[1]=b->z;p22[1]=b->depth + b->z;


            break;
        case G_BP_COLLISION_FRONT:
            mt=b->z;
            start = box.z + box.depth;
            p11[0]=box.x;p12[0]=box.width + box.x;
            p21[0]=b->x;p22[0]=b->width + b->x;
            p11[1]=box.y;p12[1]=box.height + box.y;
            p21[1]=b->y;p22[1]=b->height + b->y;

            break;
        case G_BP_COLLISION_RIGHT:
            mt=b->x;
            start = box.x + box.width;
            p11[0]=box.z;p12[0]=box.depth + box.z;
            p21[0]=b->z;p22[0]=b->depth + b->z;
            p11[1]=box.y;p12[1]=box.height + box.y;
            p21[1]=b->y;p22[1]=b->height + b->y;

            break;

        }
        end = start + dist;
        inOutDistance = 0;
        bool wcond = true;
        gf32 j = start;
        while(wcond){

            diff = mt - j;

            if(diff == 0){
                for(gu32 r=0;r<2;r++){
                    cond[r]=(p11[r] >= p21[r]) &&  (p11[r] <= p22[r]);
                    cond[r]= cond[r] || (p12[r] >=p21[r]) && (p12[r] <=p22[r]);
                }
                if(cond[0] && cond[1]){
                    //we have collision
                    if(collidedWith)*collidedWith = b;
                    if(collisionItemIndex)*collisionItemIndex = i;
                    return true;
                }

            }
            if(dist < 0){
                j-=increment;
                wcond = j >= end;
            }else{
                j+=increment;
                wcond = j <= end;
            }
            if(wcond)
                inOutDistance +=increment;
        }

    }

    return false;
}

gBpBox *gBpTools::minBox(const gList<gBpBox> &boxList){
    gBpBox *r = 0,*b;
    gf32 maxFloat = 100;
    gVector3f v(maxFloat,maxFloat,maxFloat);
    for(gu32 i=0;i<boxList.size();i++){
        b = boxList.pValue(i);

        if(b->width < v.x || b->height<v.y || b->depth<v.z){
            v.x = b->width; v.y = b->height; v.z = b->depth;
            r = b;
        }
    }
    return r;
}
