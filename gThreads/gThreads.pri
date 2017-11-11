
HEADERS += \
    $$S_GFW_DIR/gThreads/impl/gThreadPrivate.h \
    $$S_GFW_DIR/gThreads/impl/gSemaphorePrivate.h \
    $$S_GFW_DIR/gThreads/impl/gMutexPrivate.h \
    $$S_GFW_DIR/gThreads/gWait.h \
    $$S_GFW_DIR/gThreads/gThread.h \
    $$S_GFW_DIR/gThreads/gSemaphore.h \
    $$S_GFW_DIR/gThreads/gMutex.h \
    $$S_GFW_DIR/gThreads/gThreads_Global.h \
    $$S_GFW_DIR/gThreads/impl/gWaitPrivate.h

SOURCES += \
    $$S_GFW_DIR/gThreads/gWait.cpp \
    $$S_GFW_DIR/gThreads/impl/gThread_Win32.cpp \
    $$S_GFW_DIR/gThreads/gThread.cpp \
    $$S_GFW_DIR/gThreads/impl/gSemaphore_Win32.cpp \
    $$S_GFW_DIR/gThreads/gSemaphore.cpp \
    $$S_GFW_DIR/gThreads/impl/gMutex_Win32.cpp \
    $$S_GFW_DIR/gThreads/impl/gMutex_Unix.cpp \
    $$S_GFW_DIR/gThreads/gMutex.cpp \
    $$S_GFW_DIR/gThreads/impl/gThread_Unix.cpp \
    $$S_GFW_DIR/gThreads/impl/gSemaphore_Unix.cpp

unix:QMAKE_CXXFLAGS += -std=c++0x -pthread
unix:LIBS += -pthread
