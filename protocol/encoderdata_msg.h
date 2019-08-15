#ifndef __ENCODERDATA_MSG_H__
#define __ENCODERDATA_MSG_H__

#include <QObject>
#include <QtCore/QDebug>
#include "msg_base.h"

class EndcoderData_Msg : public Msg_Base
{

public:

    explicit EndcoderData_Msg(QObject *parent = 0);

    bool fromByteArray(QByteArray arr);

    QByteArray toByteArray();

    qint16 getEncoderData(quint8 index);

private:
    qint16 enc_data[4];
};

#endif
