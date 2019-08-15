#-------------------------------------------------
#
# Project created by QtCreator 2014-08-15T14:34:50
#
#-------------------------------------------------

QT       += core gui serialport printsupport network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = rt-assistant
TEMPLATE = app
#设置语言为C++
LANGUAGE = C++
SOURCES += $$PWD/main.cpp\
           $$PWD/frmmain.cpp \
           $$PWD/crc32.c \
           $$PWD/searchfrm.cpp \
           $$PWD/Communication.cpp \
           $$PWD/firmware.cpp


HEADERS  += $$PWD/frmmain.h \
            $$PWD/crc32.h \
            $$PWD/searchfrm.h \
            $$PWD/Communication.h \
            $$PWD/firmware.h



FORMS    += $$PWD/frmmain.ui \
            $$PWD/searchfrm.ui

TRANSLATIONS = $$PWD/lang_en.ts \
               $$PWD/lang_cn.ts

# Platform Specific
include($$PWD/../protocol/protocol.pri)
include($$PWD/../qui/qui.pri)
include($$PWD/../third_party/third_party.pri)
include($$PWD/../widgets/widgets.pri)

INCLUDEPATH += $$PWD


MOC_DIR         = temp/moc
RCC_DIR         = temp/rcc
UI_DIR          = temp/ui
OBJECTS_DIR     = temp/obj
DESTDIR         = bin


