#ifndef GTHREADS_GLOBAL_H
#define GTHREADS_GLOBAL_H
#ifdef WIN32
#ifdef SHARED_LIBRARY
#ifdef SHARED_EXPORT
#define GTHREADS_LIB __declspec(dllexport)
#else
#define GTHREADS_LIB __declspec(dllimport)
#endif
#else
#define GTHREADS_LIB
#endif
#else
#define GTHREADS_LIB
#endif

#include <gBase/gtypes.h>
#endif // GTHREADS_GLOBAL_H
