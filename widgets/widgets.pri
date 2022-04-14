# =================================================
# * This file is part of the rt_assistant project
# * Copyright (C) 2018 - 2019 nongxiaoming
#
# =================================================

INCLUDEPATH += $$PWD

SOURCES += \
    $$PWD/api/iconhelper.cpp \
    $$PWD/api/app.cpp \
    $$PWD/api/appinit.cpp \
    $$PWD/frmmessagebox.cpp \
    $$PWD/frminputbox.cpp\
    $$PWD/checkboxdelegate.cpp \
    $$PWD/ipaddress.cpp \
    $$PWD/progressbardelegate.cpp \
    $$PWD/progressbarwait.cpp \
    $$PWD/switchbutton.cpp

HEADERS  += \
    $$PWD/api/iconhelper.h \
    $$PWD/api/uihelper.h \
    $$PWD/api/app.h \
    $$PWD/api/appinit.h \
    $$PWD/frmmessagebox.h \
    $$PWD/frminputbox.h \
    $$PWD/checkboxdelegate.h \
    $$PWD/ipaddress.h \
    $$PWD/progressbardelegate.h \
    $$PWD/progressbarwait.h \
    $$PWD/switchbutton.h 


FORMS    +=  \
    $$PWD/frmmessagebox.ui \
    $$PWD/frminputbox.ui \

win32:RC_FILE   = $$PWD/other/main.rc

RESOURCES += \
    $$PWD/other/rc.qrc
