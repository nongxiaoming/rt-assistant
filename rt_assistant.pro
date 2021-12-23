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


TRANSLATIONS = src/lang_en.ts \
               src/lang_cn.ts

# Platform Specific
include($$PWD/assistant/assistant.pri)
include($$PWD/widgets/widgets.pri)
include($$PWD/third_party/third_party.pri)
include($$PWD/utils/utils.pri)
include($$PWD/version/version.pri)

INCLUDEPATH += $$PWD/src


MOC_DIR         = temp/moc
RCC_DIR         = temp/rcc
UI_DIR          = temp/ui
OBJECTS_DIR     = temp/obj
DESTDIR         = bin

