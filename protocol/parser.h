#ifndef __PARSER_H__
#define __PARSER_H__

#include <QObject>
#include "checksum.h"
#include "protocol.h"
#include <QtNetwork>

typedef enum
{
   STATE_WAIT_SYNC1 = 0x00,
   STATE_WAIT_SYNC2,
   STATE_GET_HEAD,
   STATE_GET_PAYLOAD,
   STATE_GET_CHECKSUM,
   STATE_GET_END,
}parse_state_t;

class LCSParser : public QObject
{
    Q_OBJECT

signals:
    void ParseDataReady(QHostAddress ip,QByteArray &data);

public:
    explicit LCSParser(QObject *parent = 0);

    void ParseData(QHostAddress &ip,QByteArray &data);

    void ParseData(QHostAddress &ip,quint8 ch);

private:
    QByteArray data_buffer;
    parse_state_t state;
    quint16 data_length;
};


#endif  /* endif __PARSER_H__ */
