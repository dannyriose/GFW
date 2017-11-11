#ifndef GBPBOX_H
#define GBPBOX_H

//! Defines a box
//!

#include <gVariant/gVariantList.h>
#include <gMath/gVector3.h>
namespace gfw{
enum G_BP_BOX_ORIENTATION{
    // width , height , depth
    GBP_BO_WHD,
    GBP_BO_WDH,
    GBP_BO_DHW,
    GBP_BO_DWH,
    GBP_BO_HWD,
    GBP_BO_HDW

};
    //! Representation of a box for the bin packing algorithm
    struct gBpBox{
        //Representation of dimensions
        //! Box width
        gf32 width;
        //! Box height
        gf32 height;
        //! Box length
        gf32 depth;
        //!X Position in the bin area
        gf32 x;
        //!Y Position in the bin area
        gf32 y;
        //!Z Position in the bin area
        gf32 z;
        //! Box current orientation
        gs32 orientation;
        //!A list of variant values to store additional data
        gVariantList addon;
        //! just for debug purposes
        bool bBoxOver;
        gList<gBpBox> steps;
        //! this for free boxes to highlight residues
        int isResidue;
        //! Initilization of variables
        gBpBox():width(0),height(0),depth(0),x(0),y(0),z(0),orientation(GBP_BO_WHD),
            bBoxOver(true),
        isResidue(false){

        }
        gBpBox(gf32 _width,gf32 _height,gf32 _depth):width(_width),
            height(_height),depth(_depth),x(0),y(0),z(0),orientation(GBP_BO_WHD),
            bBoxOver(true),
        isResidue(false){

        }

        gBpBox(const gBpBox &other):width(other.width),
            height(other.height),
            depth(other.depth),
            x(other.x),
            y(other.y),
            z(other.z),
            addon(other.addon),
            bBoxOver(other.bBoxOver),
            steps(other.steps),
            isResidue(other.isResidue){

        }
        void setPosition(gf32 _x,gf32 _y,gf32 _z){
            x=_x;y=_y;z=_z;
        }
        void setPosition(const gVector3f &v){
            x=v.x;y=v.y;z=v.z;
        }
        void setDimensions(gf32 _w,gf32 _h,gf32 _d){
            width = _w;height = _h; depth = _d;
        }

        gf32 volume() const{
            return width*depth*height;
        }
        //! Operator == returns if the box is identical to another including position
        bool operator == (const gBpBox &other) const{
            bool retd = (width == other.width) && (height == other.height) && (depth == other.depth);
            bool retp = (x == other.x && y == other.y && z == other.z);
            return retd && retp;
        }
        //! Returns truw if boxes have similar dimensions
        bool sameDimensions(const gBpBox &other,gs32 *outorientation) const{
            gs32 orient = -1;
            //widthxheightxdepth

            if(*this == other){
                orient = GBP_BO_WHD;
            }else if(width == other.width &&
                     height == other.depth &&
                     depth == other.height){ //widthxdepthxheight
                orient = GBP_BO_WDH;
            }else if(width == other.height &&
                     height == other.width &&
                     depth == other.depth){ // height x width x depth
                orient = GBP_BO_HWD;

            }else if(width == other.height &&
                     height == other.depth &&
                     depth == other.width){ // height x depth x width
                orient = GBP_BO_HDW;

            }else if(width == other.depth &&
                     height == other.height &&
                     depth == other.width){ // depth x height x width
                orient = GBP_BO_DHW;

            }else if(width == other.depth &&
                     height == other.width &&
                     depth == other.height){ // depth x width x height
                orient = GBP_BO_DWH;

            }

            if(outorientation)*outorientation = orient;
            return orient!=-1;
        }
    };
}

#endif // GBPBOX_H
