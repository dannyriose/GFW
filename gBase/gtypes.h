/*************************************GTYPES***************************************************/
/*          FILE: gtypes.h
            DESCRIPTION: Contains definitions of the basic data types using by the g-framework
*/
#ifndef GTYPES_H
#define GTYPES_H
#include <stdint.h>

//defines all basic data types for the g-framework
#ifndef MIN
#define MIN(a,b) (a  < b ? a : b)
#endif
#ifndef MAX
#define MAX(a,b) (a  > b ? a : b)
#endif
#ifndef MAX_PATH
#define MAX_PATH 260
#endif
#ifdef _WIN32
    #ifdef _WIN64
        #define G_SYSTEM_64
    #else
        #define G_SYSTEM_32
    #endif
#endif
#ifdef MSVC
#define ginline __forceinline
#else
#define ginline inline
#endif
#ifdef __GNUC__
#if defined( __x86_64__ )|| defined(__ppc64__)
#define G_SYSTEM_64
#else
#define G_SYSTEM_32
#endif
#endif

#ifdef __GNUC__
#define g_restrict __restrict__
#endif
#ifdef _MSC_VER
#define g_restrict __restrict
#endif
//Define an useful macro for random number generation
#define RAND_RANGE(__n, __min, __max, __tmax) \
    (__n) = (__min) + (long) ((double) ( (double) (__max) - (__min) + 1.0) * ((__n) / ((__tmax) + 1.0)))
#define RAND_RANGEF(__n, __min, __max, __tmax) \
    (__n) = (__min) + ((double) ( (double) (__max) - (__min) + 1.0) * ((__n) / ((__tmax) + 1.0)))
//Each type self explanatory
namespace gfw
{
    typedef int8_t gs8;
    typedef uint8_t gu8;
    typedef int16_t gs16;
    typedef uint16_t gu16;
    typedef int32_t gs32;
    typedef uint32_t gu32;
    typedef int64_t gs64;
    typedef uint64_t gu64;
    typedef float gf32;
    typedef double gf64;
    typedef void *gHandle;
    typedef uintptr_t guptr;
 #ifdef DOUBLE_PRESICION
    typedef gf64 gScalar;
#else
    typedef gf32 gScalar;
#endif
#ifdef G_SYSTEM_32
    typedef gu32 gsize;
#endif
#ifdef G_SYSTEM_64
    typedef gu64 gsize;
#endif
    //! Search operators
    enum G_SEARCH_OPERATOR{
        G_SEARCH_EQUAL,
        G_SEARCH_NOTEQUAL,
        G_SEARCH_LESSTHAN,
        G_SEARCH_GREATERTHAN,
        G_SEARCH_LESSOREQUAL,
        G_SEARCH_GREATEROREQUAL
    };
    //! Macro to help operator selectors
#define G_OP_EQ(op,term1,term2,ret) switch(op){case G_SEARCH_EQUAL:ret=term1==term2;break; \
                                               case G_SEARCH_NOTEQUAL:ret=term1!=term2;break;\
                                               default:ret=false;break;}
//! Macro to help getting the private members
//!
#define GFW_PRIVATE_CAST(var,member,type) type *var=static_cast<type *>(member)
    // #define GFW_PRIVATE_CAST(var,member,type) type *var=(type *)member
#define GFW_PRIVATE_CREATE(var,member,type) type *var=new type;member=var

    //! Prototype callback function used as 'cleaning' function for elements contained in gArray
    typedef void (*IClean)(void *ptr);
    //! prototype callback function used as 'allocator' function for elements contained on dynamic structures such as gArray
    typedef void *(*IAlloc)(const void *obj);
#define GFW_UNUSED(v) (void)v
}
#endif // GTYPES_H
