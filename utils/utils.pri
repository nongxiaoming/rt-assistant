include ($$PWD/mousetap/mousetap.pri)

HEADERS += \
    $$PWD/compat.h \
    $$PWD/bufferutil.h \
    $$PWD/crc32.h \
    $$PWD/config.h

SOURCES += \
    $$PWD/bufferutil.cpp \
    $$PWD/crc32.c \
    $$PWD/config.cpp

INCLUDEPATH += \
        $$PWD
