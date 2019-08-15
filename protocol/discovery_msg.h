#ifndef __DISCOVERY_MSG_H__
#define __DISCOVERY_MSG_H__

#include <QObject>
#include <QtCore/QDebug>
#include "msg_base.h"


class Discovery_Msg : public Msg_Base
{
 //   Q_OBJECT


public:
    explicit Discovery_Msg(QObject *parent = 0);

    bool fromByteArray(QByteArray arr);

    QByteArray toByteArray();

    QString Name;

    QString SN;

    quint8 mac[6];

    quint32 sw_ver;

    quint32 build_date;

private:

};

#endif
