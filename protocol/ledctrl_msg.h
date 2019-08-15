#ifndef __LEDCTRL_MSG_H__
#define __LEDCTRL_MSG_H__

#include <QObject>
#include <QtCore/QDebug>
#include "msg_base.h"


class LedCtrl_Msg : public Msg_Base
{
 //   Q_OBJECT


public:
    explicit LedCtrl_Msg(QObject *parent = 0);

    bool fromByteArray(QByteArray arr);

    QByteArray toByteArray();

    bool setState(quint8 index ,quint8 state);

    quint8 getState(quint8 index)
    {
        if(index <60)
        {
            return led_state[index];
        }else
        {
            return 0;
        }
    }
private:
    quint8 led_state[60];
};

#endif
