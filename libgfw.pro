#Define here where the GFW files are
win32:S_GFW_DIR=D:/dprograms/clibs #define the working directory of the G-Framework
unix:S_GFW_DIR=/media/danny/DATA/dprograms/clibs/
#Define where curl lib is
unix:S_CURL_DIR=/media/danny/DATA/Dev/extern/curl-pri

CONFIG+=GFW_STATICLIB
TEMPLATE=lib
GFW_STATICLIB{
    CONFIG+=staticlib

}else{
    DEFINES+=SHARED_LIBRARY SHARED_EXPORT

    win32:LIBS += Shell32.lib User32.lib
}
QT-=core gui

CONFIG(release, debug|release){
   GFW_GUDA{
        TARGET = gfw_guda
   }else{
        TARGET = gfw
   }
}
else{
    CONFIG(debug, debug|release){
         GFW_GUDA{
             TARGET = gfwd_guda
         }else{
             TARGET = gfwd
        }
    }
}



GFW_GUDA{
DEFINES+=INCLUDE_GUDA
}
win32:QMAKE_LFLAGS+=/INCREMENTAL:NO
unix:QMAKE_CXXFLAGS+= -Werror=return-type
include(gfw.pri)

DISTFILES += \
    README.md

