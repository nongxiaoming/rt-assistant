# =================================================
# * This file is part of the rt_assistant project
# * Copyright (C) 2018 - 2019 nongxiaoming
#
# =================================================

INCLUDEPATH += $$PWD/aes
INCLUDEPATH += $$PWD/qcustomplot
INCLUDEPATH += $$PWD/qhexedit
INCLUDEPATH += $$PWD/log4qt

SOURCES += \
    $$PWD/aes/aes.c \
    $$PWD/qhexedit/commands.cpp\
    $$PWD/qhexedit/qhexedit.cpp\
    $$PWD/qhexedit/qhexedit_p.cpp\
    $$PWD/qhexedit/xbytearray.cpp

HEADERS  += \
    $$PWD/aes/aes.h \
    $$PWD/aes/aes_config.h \
    $$PWD/qhexedit/commands.h\
    $$PWD/qhexedit/qhexedit.h\
    $$PWD/qhexedit/qhexedit_p.h\
    $$PWD/qhexedit/xbytearray.h

include($$PWD/log4qt/log4qt.pri)

include($$PWD/dap/dap.pri)
