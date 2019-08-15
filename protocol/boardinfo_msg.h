#ifndef __BOARDINFO_MSG_H__
#define __BOARDINFO_MSG_H__

#include <QObject>
#include <QtCore/QDebug>
#include "msg_base.h"

class BoardInfo_Msg : public Msg_Base
{
 //   Q_OBJECT

public:
    explicit BoardInfo_Msg(QObject *parent = 0);

    bool fromByteArray(QByteArray arr);

    QByteArray toByteArray();

private:

};

#endif
