# =================================================
# * This file is part of the LCS project
# * Copyright (C) 2015 - 2018 nongxiaoming
#
# =================================================

INCLUDEPATH += $$PWD

SOURCES += \
    $$PWD/parser.cpp \
    $$PWD/checksum.cpp \
    $$PWD/msg_base.cpp \
    $$PWD/heartbeat_msg.cpp \
    $$PWD/boardinfo_msg.cpp \
    $$PWD/ledctrl_msg.cpp \
    $$PWD/keystate_msg.cpp \
    $$PWD/faderdata_msg.cpp \
    $$PWD/encoderdata_msg.cpp \
    $$PWD/upgraderesponse_msg.cpp \
    $$PWD/upgradedata_msg.cpp \
    $$PWD/upgraderequest_msg.cpp \
    $$PWD/discovery_msg.cpp \
    $$PWD/network_msg.cpp

HEADERS  += \
    $$PWD/protocol.h \
    $$PWD/parser.h \
    $$PWD/checksum.h \
    $$PWD/msg_base.h \
    $$PWD/heartbeat_msg.h \
    $$PWD/boardinfo_msg.h \
    $$PWD/ledctrl_msg.h \
    $$PWD/keystate_msg.h \
    $$PWD/faderdata_msg.h \
    $$PWD/encoderdata_msg.h \
    $$PWD/upgradedata_msg.h \
    $$PWD/upgraderesponse_msg.h \
    $$PWD/upgraderequest_msg.h \
    $$PWD/discovery_msg.h \
    $$PWD/network_msg.h
