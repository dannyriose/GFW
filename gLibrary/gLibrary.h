#ifndef GLIBRARY_H
#define GLIBRARY_H
#ifdef WIN32
#ifdef SHARED_LIBRARY
#ifdef SHARED_EXPORT
#define SHARED_GLIBRARY __declspec(dllexport)
#else
#define SHARED_GLIBRARY __declspec(dllimport)
#endif
#else
#define SHARED_GLIBRARY
#endif
#else
#define SHARED_GLIBRARY
#endif
#include <gBase/gtypes.h>
#include <gString/gString.h>
namespace gfw
{
    class SHARED_GLIBRARY gLibrary
    {
        void *d;
    public:
        gLibrary();
        ~gLibrary();
        bool load(const gString &file);
        void unload();
        void *resolve(const gString &func) const;
        bool isLoaded() const;
        const gString &path() const;//get the path of the library
    };
}
#endif // GLIBRARY_H
