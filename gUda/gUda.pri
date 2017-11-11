
HEADERS += \
    $$S_GFW_DIR/gUda/gUda.h

SOURCES += \
    $$S_GFW_DIR/gUda/gUda.cpp

win32:include($$S_CURL_DIR/curl.pri)
