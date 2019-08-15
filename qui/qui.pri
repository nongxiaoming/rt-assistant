# =================================================
# * This file is part of the rt_assistant project
# * Copyright (C) 2018 - 2019 nongxiaoming
#
# =================================================

INCLUDEPATH += $$PWD
INCLUDEPATH += $$PWD/usercontrol

SOURCES += \
    $$PWD/api/iconhelper.cpp \
    $$PWD/api/app.cpp \
    $$PWD/api/appinit.cpp \
    $$PWD/usercontrol/frmmessagebox.cpp \
    $$PWD/usercontrol/frminputbox.cpp\

HEADERS  += \
    $$PWD/api/iconhelper.h \
    $$PWD/api/myhelper.h \
    $$PWD/api/app.h \
    $$PWD/api/appinit.h \
    $$PWD/usercontrol/frmmessagebox.h \
    $$PWD/usercontrol/frminputbox.h

FORMS    +=  \
    $$PWD/usercontrol/frmmessagebox.ui \
    $$PWD/usercontrol/frminputbox.ui \

win32:RC_FILE   = $$PWD/other/main.rc

RESOURCES += \
    $$PWD/other/rc.qrc
