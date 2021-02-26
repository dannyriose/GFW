#ifndef GIMAGEPRIVATE_H
#define GIMAGEPRIVATE_H
#include <gImage/gImage.h>
namespace gfw{

    struct gImagePrivate:public gSharedObject{
        //! Image data
        gByteArray data;
        //! The color table if any
        gUIntArray colorTable;
        //! The image bit depth
        gs32 bitDepth;
        //! width
        gs32 width;
        //! Height of the image
        gs32 height;
        //! The width of the image
        gs32 lineWidth;

        //! The constructor
        gImagePrivate():bitDepth(0),
            width(0),
            height(0),
            lineWidth(0){

        }
        //! Destructor
        ~gImagePrivate(){

        }
        //! copy function
        virtual void copy(const gSharedObject *other){
            const gImagePrivate *o=static_cast<const gImagePrivate *>(other);
            data.clear();
            colorTable.clear();
            data=o->data;
            colorTable=o->colorTable;
            width=o->width;
            height=o->height;
            lineWidth=o->lineWidth;
            bitDepth=o->bitDepth;
        }
        //
        gu32 pixel1Bit(gs32 x, gs32 y) const;
        gu32 pixel4Bit(gs32 x, gs32 y) const;
        gu32 pixel7and8Bit(gs32 x,gu32 y) const;
        gu32 pixel16bit(gs32 x,gs32 y) const;
        gu32 pixel24bit(gs32 x,gs32 y) const;

        void setPixel1Bit(gs32 x,gs32 y,gu32 pixel);
        void setPixel4Bit(gs32 x,gs32 y,gu32 pixel);
        void setPixel7and8(gs32 x,gs32 y,gu32 pixel);
        void setPixel16Bit(gs32 x,gs32 y,gu32 pixel);
        void setPixel24Bit(gs32 x,gs32 y,gu32 pixel);
    };
    G_SHAREDOBJECT_FACTORY(gImagePrivateFactory,gImagePrivate);
}
#endif // GIMAGEPRIVATE_H
