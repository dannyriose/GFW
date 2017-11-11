
HEADERS += \
    $$S_GFW_DIR/gLibrary/gLibrary.h

SOURCES += \
    $$S_GFW_DIR/gLibrary/gLibrary_Win.cpp \
    $$S_GFW_DIR/gLibrary/gLibrary_Unix.cpp

unix:LIBS+= -ldl
