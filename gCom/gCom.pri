 

HEADERS += \
    $$S_GFW_DIR/gCom/gSerialRS232.h \
    $$S_GFW_DIR/gCom/gSerialRS232Port.h \
    gCom/impl/gSerialRS232PortPrivate.h \
    gCom/impl/gSerialRS232Private.h

SOURCES += \
    gCom/gSerialRS232Port.cpp \
    gCom/gSerialRS232.cpp \
    gCom/impl/gSerialRS232Port_Win32.cpp \
    gCom/impl/gSerialRS232Port_Unix.cpp \
    gCom/impl/gSerialRS232_Win32.cpp
