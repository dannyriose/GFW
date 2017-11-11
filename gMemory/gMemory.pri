INCLUDEPATH+=$$S_GFW_DIR/gMemory
INCLUDEPATH+=$$S_GFW_DIR/gbase
HEADERS += \
    $$S_GFW_DIR/gMemory/gMemory.h \
    $$S_GFW_DIR/gMemory/gBaseArray.h \
    gMemory/gMemoryPrivate.h \
    gMemory/gSharedPointer.h

SOURCES += \
    $$S_GFW_DIR/gMemory/gMemory.cpp
