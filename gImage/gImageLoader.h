#ifndef GIMAGELOADER_H
#define GIMAGELOADER_H
#include "gImage.h"
#include <gString/gStringList.h>
namespace gfw{
    class gImageLoader{
    protected:
        //! Image extensions it supports
        gStringList m_imageExtensions;
        //! Image format
        gs32 m_format;
    public:
        gImageLoader(){}
        virtual ~gImageLoader();
        virtual gImage load(const gString &file) const = 0;
        virtual bool save(const gString &file, const gImage &image) const = 0;
        const gStringList &imageExtensions() const{return m_imageExtensions;}
    };
}

#endif // GIMAGELOADER_H
