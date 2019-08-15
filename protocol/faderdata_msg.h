#ifndef __FADERDATA_MSG_H__
#define __FADERDATA_MSG_H__

#include <QObject>
#include <QtCore/QDebug>
#include "msg_base.h"

class FaderData_Msg : public Msg_Base
{

public:
    explicit FaderData_Msg(QObject *parent = 0);

    bool fromByteArray(QByteArray arr);

    QByteArray toByteArray();

    quint16 getValue(quint8 index)
    {
        if(index < 15)
        {
          return  fader_data[index];
        }else
        {
          return  0;
        }
    }


private:
    quint16 fader_data[15];
};

#endif
