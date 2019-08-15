#ifndef __UPGRADEDATA_MSG_H__
#define __UPGRADEDATA_MSG_H__

#include <QObject>
#include <QtCore/QDebug>
#include "msg_base.h"


class UpgradeData_Msg : public Msg_Base
{
 //   Q_OBJECT


public:
    explicit UpgradeData_Msg(QObject *parent = 0);

    bool fromByteArray(QByteArray arr);

    QByteArray toByteArray();

    bool set_pack_index(quint16 index)
    {

        this->pack_index = index;

        return true;
    }

    quint16 get_pack_index()
    {
      return this->pack_index;
    }
    bool set_pack_data(quint16 index,QByteArray &fw_data)
    {

        this->pack_index = index;
        this->payload.clear();
        this->payload.append((quint8)(this->pack_index&0xff));
        this->payload.append((quint8)((this->pack_index>>8)&0xff));
        this->payload.append(fw_data);

        return true;
    }

    quint16 pack_index;
private:

};

#endif
