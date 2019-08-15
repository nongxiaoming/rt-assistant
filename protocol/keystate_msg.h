#ifndef __KEYSTATE_MSG_H__
#define __KEYSTATE_MSG_H__

#include <QObject>
#include <QtCore/QDebug>
#include "msg_base.h"


class KeyState_Msg : public Msg_Base
{
 //   Q_OBJECT


public:
    explicit KeyState_Msg(QObject *parent = 0);

    bool fromByteArray(QByteArray arr);

     QByteArray toByteArray();

     quint8 getState(quint8 index)
     {
         if(index <60)
         {
             return this->key_state[index];
         }else
         {
             return 0;
         }
     }
private:
    quint8 key_state[60];
};

#endif
