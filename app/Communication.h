#ifndef __COMMUNICATION_H__
#define __COMMUNICATION_H__

#include <QObject>
#include <QtSerialPort>
#include <QtNetwork>
#include "parser.h"
#include "ledctrl_msg.h"
#include "keystate_msg.h"
#include "heartbeat_msg.h"
#include "faderdata_msg.h"
#include "encoderdata_msg.h"
#include "boardinfo_msg.h"
#include "upgradedata_msg.h"
#include "upgraderequest_msg.h"
#include "upgraderesponse_msg.h"
#include "discovery_msg.h"
#include "msg_base.h"

class Communication : public QObject
{
    Q_OBJECT

signals:
    void MsgReady(quint8 board_id,QHostAddress &ip,QByteArray &data);

public:
    explicit Communication(QObject *parent = 0);

    void init();

    int SendDataBroadcast(QByteArray arr);

    int SendData(QHostAddress &to,QByteArray arr);

    static Communication *Instance() {
        if (!_instance) {
          _instance = new Communication;
          _instance->init();
        }
        return _instance;
    }
   static Communication *_instance;
private slots:
    void readyRead();
    void ParseDataReady(QHostAddress ip,QByteArray &data);

private:
   LCSParser *parser;
   QList<QUdpSocket*> UdpList;

   quint8 board_id;
   QHostAddress curr_ip;
};

#endif  /* endif __COMMUNICATION_H__ */
