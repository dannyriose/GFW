win32:S_GFW_DIR=D:/dprograms/clibs #define the working directory of the G-Framework
unix:S_GFW_DIR=/var/run/media/daniel/DATA/dprograms/clibs
unix:S_CURL_DIR=/var/run/media/daniel/DATA/Dev/extern/curl-pri

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

