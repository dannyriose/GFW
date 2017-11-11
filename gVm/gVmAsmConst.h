#ifndef GVMASMCONST_H
#define GVMASMCONST_H

namespace gfw
{
    enum GVMASM_KWCONST
    {
        KEY_STACKSIZE=256,
        KEY_HEAPSIZE,
        KEY_MAIN,
        KEY_VARS,
        KEY_PROC,
        KEY_INT,
        KEY_FLOAT,
        KEY_STRING,
        KEY_RACC,
        KEY_RC,
        KEY_RP,
        KEY_STACK,//reference of stack
        KEY_STACKPOINTER,//reference of stackpointer
        KEY_STACKDIRECT,//reference to stack direct from 0
        KEY_RGEN,
        KEY_APICALL,
        KEY_INCLUDE,
        KEY_VAR //added for single var declaration on functions progs

    };
}

#endif // GVMASMCONST_H
