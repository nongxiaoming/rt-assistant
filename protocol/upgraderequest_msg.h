#ifndef __UPGRADEREQUEST_MSG_H__
#define __UPGRADEREQUEST_MSG_H__

#include <QObject>
#include <QtCore/QDebug>
#include "msg_base.h"


class UpgradeRequest_Msg : public Msg_Base
{
 //   Q_OBJECT


public:
    explicit UpgradeRequest_Msg(QObject *parent = 0);

    bool fromByteArray(QByteArray arr);

    QByteArray toByteArray();

    quint8  type;
    quint16 oem_code;
    quint8 dev_id[12];
    char pip[32];
    quint32 sw_ver;
    quint32 build_utc;

private:

};

#endif
