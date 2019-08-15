#ifndef __HEARTBEAT_MSG_H__
#define __HEARTBEAT_MSG_H__

#include <QObject>
#include <QtCore/QDebug>
#include "msg_base.h"


class HeartBeat_Msg : public Msg_Base
{
 //   Q_OBJECT


public:
    explicit HeartBeat_Msg(QObject *parent = 0);

    bool fromByteArray(QByteArray arr);

    QByteArray toByteArray();

    quint32 sys_tick;
private:

};

#endif
