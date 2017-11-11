#ifndef GFW_EXPORT_H
#define GFW_EXPORT_H
#ifdef WIN32
#ifdef SHARED_LIBRARY
#ifdef SHARED_EXPORT
#define SHARED_GFW __declspec(dllexport)
#else
#define SHARED_GFW __declspec(dllimport)
#endif
#else
#define SHARED_GFW
#endif
#else
#define SHARED_GFW
#endif
#endif // GFW_EXPORT_H
