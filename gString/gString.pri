#include($$S_GFW_DIR/gBase/gbase.pri)
#include($$S_GFW_DIR/gMemory/gmemory.pro)

HEADERS += \
    $$S_GFW_DIR/gString/gString.h \
     $$S_GFW_DIR/gString/gStringList.h \
     $$S_GFW_DIR/gString/impl/gStringPrivate.h

SOURCES += \
    $$S_GFW_DIR/gString/gString.cpp \
    $$S_GFW_DIR/gString/gStringList.cpp
