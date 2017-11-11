#ifndef GIMAGE_H
#define GIMAGE_H
#include <gfw_global.h>
#include <gMemory/gBaseArray.h>
#include <gString/gString.h>
namespace gfw{
    enum GIMAGE_FORMAT{
        GIMAGE_FORMAT_BMP,
        GIMAGE_FORMAT_JPG,
        GIMAGE_FORMAT_PNG,
        GIMAGE_FORMAT_GIF,
        GIMAGE_FORMAT_EXTENSION
    };
    enum GIMAGE_BITDEPTH{
        GIMAGE_BITDEPTH_1=1,
        GIMAGE_BITDEPTH_4=4,
        GIMAGE_BITDEPTH_7=7,
        GIMAGE_BITDEPTH_8=8,
        GIMAGE_BITDEPTH_16=16,
        GIMAGE_BITDEPTH_24=24,
        GIMAGE_BITDEPTH_32=32
    };
    //! Utility class to manage image bitmaps
    class SHARED_GFW gImage:public gBaseShared{
    public:
        gImage();
        gImage(const gImage &other);
        ~gImage();
        gImage &operator = (const gImage &other);

        bool alloc(gu32 _size);
        bool create(gs32 _width,gs32 _height,gs32 _bitDepth,
                    const gByteArray &_data,
                    const gUIntArray &_colortable,
                    bool bShared);
        gs32 width() const;
        gs32 height() const;
        gs32 bitDepth() const;

        gu32 size() const;

        const gByteArray &data() const;
        gByteArray &data();

        gUIntArray &colorTable();
        const gUIntArray &colorTable() const;

        gu32 pixel(gs32 x, gs32 y) const;
        void setPixel(gs32 x, gs32 y, gu32 color);
        gu8 *scanLine(gs32 _line) const;

        bool save(const gString &file,GIMAGE_FORMAT _format);
        bool open(const gString &file,GIMAGE_FORMAT _format = GIMAGE_FORMAT_EXTENSION);

        gImage convert(gs32 _bitdepth) const;
        void converThis(gs32 _bitdepth);

        //! Converts the image data to a float array.
        /** \param bNormalize: Normalizes color data to 1.
         * \param bAlpha: Includes alpha channel data into array.*/
        gFloatArray toFloat(bool bNormalize, bool bAlpha) const;
#ifdef U_QT
        QImage toQImage() const;
#endif
        virtual bool isEmpty() const;

    };
}
#endif // GIMAGE_H
