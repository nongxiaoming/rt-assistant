#ifndef __FIRMWARE_H
#define __FIRMWARE_H

#include <QObject>
#include <QDateTime>
#include "protocol.h"
#include <QtNetwork>

#define PACK_MAX_LEN (512)

typedef enum
{
 UPGRADERESULT_TIMEOUT = -1,
 UPGRADERESULT_FAIL = 0,
 UPGRADERESULT_SUCESS = 1,
} UpgradeResult;

class Firmware :  public QObject
{
    Q_OBJECT

signals:
    void UpgradeProgress(int);

    void UpgradeFinish(int result);

    void UpgradeOutput(QString info);

public slots:

public:
    quint32 total_size;

    quint32 fw_version;

    quint32 build_utc;

    quint8 dev_type;

    quint8 curr_board_id;

    explicit Firmware(QObject *parent = nullptr);

    bool Load(char *fw_data,quint32 file_size);

    QString getVersionString()
    {
        return  QString("%1.%2.%3").arg((this->fw_version>>24)&0xff).arg((this->fw_version>>16)&0xff).arg(this->fw_version&0xffff);
    }
    QString getBuildDateTimeShortString()
    {
        return QDateTime::fromTime_t(this->build_utc).toString("yyyy-MM-dd");
    }
    QString getBuildDateTimeString()
    {
        return QDateTime::fromTime_t(this->build_utc).toString("yyyy-MM-dd hh:mm:ss");
    }
    int getUpgradeProgress()
    {
        return (int)(finish_size*100.0/this->total_size);
    }
    void setDeviceIP(QHostAddress &ip)
    {
        this->DeviceIP = ip;
    }
    QHostAddress getDeviceIP()
    {
        return this->DeviceIP;
    }
    void setUserIndex(int ix)
    {
        this->usr_index = ix;
    }
    int getUserIndex()
    {
        return  this->usr_index;
    }
    bool isStart()
    {
        return  this->start;
    }
    void UpgradeTimeout();

    void UpgradeResponse(quint8 cmd,QByteArray &in_arr);

    bool UpgradeStart();

private:

    void write_fw_data(quint16 pack_index);

    void process_cal();
    quint8 fw_count;

    QByteArray firmware;

    quint16 current_pack;

    quint8 update_step;

    quint8 retry;

    quint32 finish_size;

    quint32 curr_fw_size;

    char *curr_fw_buffer;

    bool start;

    QHostAddress DeviceIP;

    int usr_index;
};

#endif // FIRMWARE_H
