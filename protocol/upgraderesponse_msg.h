#ifndef __UPGRADERESPONE_MSG_H__
#define __UPGRADERESPONE_MSG_H__

#include <QObject>
#include <QtCore/QDebug>
#include "msg_base.h"


class UpgradeRespone_Msg : public Msg_Base
{
 //   Q_OBJECT


public:
    explicit UpgradeRespone_Msg(QObject *parent = 0);

    bool fromByteArray(QByteArray arr);

    QByteArray toByteArray();

    char pip[32];
    quint32 sw_ver;
    quint32 build_utc;
    quint32 fw_size;

private:

};

#endif
