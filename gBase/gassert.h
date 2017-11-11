#ifndef GASSERT_H
#define GASSERT_H
//Defines the assert macro
#include <assert.h>

#define gassert(cond,msg) assert(cond && msg)
#endif // GASSERT_H
