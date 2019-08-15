#ifndef __MSG_BASE_H__
#define __MSG_BASE_H__

#include <QObject>
#include <QtCore/QDebug>
#include "protocol.h"
#include "checksum.h"

class Msg_Base : public QObject
{
 //   Q_OBJECT


public:
    explicit Msg_Base(QObject *parent = 0);

    void test()
    {
        return;
    }
    virtual bool fromByteArray(QByteArray arr)
    {
     //qDebug() << "Array:"<<arr.toHex();
     if(arr[0]!=FRAME_SYNC1||arr[1]!=FRAME_SYNC2)
     {
         return false;
     }
     this->board_id = arr[2];
     this->cmd = arr[3];
     int len = (arr[4]&0xff) + (arr[5]&0xff)*256;
     if(len > 0)
     {
     this->payload.clear();
     this->payload = arr.mid(6,len);
     }
     //qDebug() << "payload:"<<payload.toHex();
     return true;
    }

    virtual QByteArray toByteArray()
    {
     QByteArray arr;
     arr.clear();
     arr.append((char)FRAME_SYNC1);
     arr.append((char)FRAME_SYNC2);
     arr.append((char)board_id);
     arr.append((char)cmd);
     int len = payload.size();
     arr.append((char)len);
     arr.append((char)((len>>8)&0xff));
     arr.append(payload);
     quint8 checksum  = getCheckSum(arr,arr.size());
     arr.append((char)(checksum));
     arr.append((char)FRAME_END);
     //qDebug() << "toArray:"<<arr.toHex();
     return arr;
    }

    quint8 board_id;

    quint8 cmd;

    QByteArray payload;

private:

};

#endif
