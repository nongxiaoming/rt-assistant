#ifndef __NETWORK_MSG_H__
#define __NETWORK_MSG_H__

#include <QObject>
#include <QtCore/QDebug>
#include "msg_base.h"
#include <QtNetwork>

class Network_Msg : public Msg_Base
{
 //   Q_OBJECT


public:
    explicit Network_Msg(QObject *parent = 0);

    bool fromByteArray(QByteArray arr);

    QByteArray toByteArray();


    bool dhcp_enable;

    quint8 mac[6];

    QHostAddress ip;

    QHostAddress gateway;

    QHostAddress netmask;

    QHostAddress dns;

private:

};

#endif
