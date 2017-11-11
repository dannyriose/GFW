#ifndef GFILE_H
#define GFILE_H


#include <gfw_global.h>
#include <gString/gString.h>
#include <gVariant/gVariant.h>
namespace gfw
{

    class SHARED_GFW gFile:public gBaseShared
    {
    public:
        enum GFILE_ACCESSMODES
        {
            GFILEAM_ACCESSREAD,
            GFILEAM_ACCESSWRITE
        };

        enum GFILE_OPENMODES
        {
            GFILEOM_BINARY,
            GFILEOM_TEXT
        };
        enum GFILE_SEEKORIGINS
        {
            GFILE_SEEKSTART,
            GFILE_SEEKCURRENT,
            GFILE_SEEKEND

        };

        gFile();
        gFile(const gFile &other);
        ~gFile();
        bool open(const gString &file,GFILE_ACCESSMODES amode, GFILE_OPENMODES omode=GFILEOM_BINARY);
        void close();
        gsize read(void *_dest, gsize blockSize, gsize count);
        //more reads
        gsize read(gMemory &out);
        gsize read(gString &out, bool hasHeaders = true,
                   gString::GSTRING_FORMAT formatout =gString::ASCII,
                   gString::GSTRING_FORMAT fileformat = gString::ASCII);
        gsize read(gVariant &out);
        gs32 readInt();
        gu32 readUInt();
        gScalar readScalar();
        gByteArray readAll();
        //! Reads the file as a formated string
        /** File should have been saved using*/
        gString readAllToString(gString::GSTRING_FORMAT format=gString::ASCII,
                                gString::GSTRING_FORMAT fileFormat=gString::ASCII);

        gsize write(const void *src,gsize blockSize,gsize _count);
        //bWriteHeaders:Significa que se escribira en entero de 32 bits el tamano del array antes del array mismo
        gsize write(const gByteArray &src, bool bWriteHeaders=true);
        gsize write(const gShortArray &src,bool bWriteHeaders=true);
        gsize write(const gIntArray &src,bool bWriteHeaders=true);
        gsize write(const gMemory &src);
        gsize write(const gString &src, bool bWriteHeaders = true);
        gsize write(const gVariant &src);
        gsize writeInt(gs32 val);//unsigned integer.very used
        gsize writeUInt(gu32 val);
        gsize writeScalar(gScalar val);

        gsize seek(gsize offset, GFILE_SEEKORIGINS origin);
        gsize position() const; //returns the current position of the file

        bool eof() const;
        bool isOpened() const;


        gFile &operator = (const gFile &other);
        //
        FILE *handle() const;//get the file handle
        const gString &fileName() const;//gets the filename
        gsize size() const;//gets the file size

        //Not really a file operation but useful for file operations
        static gString appendExt(const gString &src, const gString &ext);
        static bool fileExists(const gString &file);
        static bool createDirectory(const gString &path);//creates a direcytory
        static gString getAppDataDirectory(const gString &appname, bool bCreateDirectory=false);

    };
}
#endif // GFILE_H
