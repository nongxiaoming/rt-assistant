#include "firmware.h"
#include <QDebug>
#include "protocol.h"
#include "Communication.h"
#include "aes.h"

Firmware::Firmware(QObject *parent) : QObject(parent)
{
    this->retry = 0;
    this->fw_count = 0;
    this->update_step  = 0;
    this->current_pack = 0;
    this->fw_version = 0;
    this->dev_type = 0;
    this->curr_board_id = 0;
    this->build_utc = 0;
    this->finish_size = 0;
    this->start = false;
}
bool Firmware::Load(char *fw_data,quint32 file_size)
{

    if( this->fw_count > 0)
    {
        char *fw_data = this->firmware.data();

        this->curr_board_id=this->firmware[2];

        this->dev_type = this->firmware[3];

        memcpy(&this->fw_version,&fw_data[8],4);

        memcpy(&this->build_utc,&fw_data[12],4);

        this->total_size = this->firmware.size();

    }else
    {

      return  false;
    }

    return  true;
}
bool Firmware::UpgradeStart()
{
    if(start)
    {
        return false;
    }
    if(this->fw_count==0)
    {
        return false;
    }

    this->update_step = 0;
    this->curr_board_id=this->firmware[2];
    this->dev_type = this->firmware[3];
    UpgradeRequest_Msg req_msg;
    req_msg.build_utc = this->build_utc;
    req_msg.sw_ver = this->fw_version;
    req_msg.board_id =  this->curr_board_id;
    Communication::Instance()->SendData(this->DeviceIP,req_msg.toByteArray());
    this->retry = 0;
    this->start = true;
    return true;
}
void Firmware::UpgradeTimeout()
{
    QByteArray arr;
    if(this->start)
    {
    this->retry++;
    qDebug()<<tr("retey=%1,step=%2").arg(this->retry).arg(this->update_step);
      if(this->update_step == 0)
      {
       qDebug()<<tr(" retry to write,retey=%1").arg(this->retry);

       UpgradeRequest_Msg req_msg;
       req_msg.build_utc = this->build_utc;
       req_msg.sw_ver = this->fw_version;
       if(this->curr_board_id!=0x01)
       {
           req_msg.board_id =  0x01;
           Communication::Instance()->SendData(this->DeviceIP,req_msg.toByteArray());
       }
       req_msg.board_id =  this->curr_board_id;
       Communication::Instance()->SendData(this->DeviceIP,req_msg.toByteArray());
      }
      if(this->update_step==1)
      {
          this->update_step = 0;
          qDebug()<<tr(" retry to write,retey=%1").arg(this->retry);

          UpgradeRequest_Msg req_msg;
          req_msg.build_utc = this->build_utc;
          req_msg.sw_ver = this->fw_version;
          req_msg.board_id =  this->curr_board_id;
          Communication::Instance()->SendData(this->DeviceIP,req_msg.toByteArray());
      }
    if(this->retry>=20)  //升级超时失败
    {
     this->start = false;
     emit UpgradeFinish(UPGRADERESULT_TIMEOUT);
    }
    }
}
void Firmware::write_fw_data(quint16 pack_index)
{
 quint32 pos = 0;
 quint32 len = 0;

 if(pack_index==0)
 {
   len = 32;
 }else
 {
   pos = (pack_index - 1)*PACK_MAX_LEN + 32;
   len = (this->curr_fw_size-pos > PACK_MAX_LEN)?PACK_MAX_LEN:this->curr_fw_size-pos;
   qDebug()<<tr("write len = %1").arg(len);
 }
 if(pos>=this->curr_fw_size)
 {
     return;
 }

 QByteArray data;
 data.append(&this->curr_fw_buffer[pos],len);
 if(pack_index!=0&&len < PACK_MAX_LEN)
 {
     for (int i = len;i < PACK_MAX_LEN;i++) {
         data.append((char)(0xff));
     }
 }
 UpgradeData_Msg msg;
 msg.board_id = this->curr_board_id;
 msg.set_pack_data(pack_index,data);
 Communication::Instance()->SendData(this->DeviceIP,msg.toByteArray());
}
void Firmware::process_cal()
{
   if(this->current_pack >0)
     {
      this->finish_size = (this->current_pack - 1)*PACK_MAX_LEN + 48;
     }
     else
     {
      this->finish_size =  48;
     }
    emit UpgradeProgress(this->finish_size*100/this->total_size);
}
void Firmware::UpgradeResponse(quint8 cmd,QByteArray &in_arr)
{
    QString info;
    switch (cmd) {
    case CMD_UPGRADE_REQ:
    {
     UpgradeRequest_Msg req_msg;
     req_msg.fromByteArray(in_arr);
     this->update_step = 1;

    QString sw_ver_str = tr("%1.%2.%3").arg((req_msg.sw_ver>>24)&0xff).arg((req_msg.sw_ver>>16)&0xff).arg(req_msg.sw_ver&0xffff);
    QString build_time_str = QDateTime::fromTime_t(req_msg.build_utc).toString("yyyy-M-d H:m");
    info = tr("发现设备,类型为%1,软件版本:%2,发布时间:%3").arg(req_msg.type,2,16,QChar('0')).arg(sw_ver_str).arg(build_time_str);
    emit UpgradeOutput(info);
    if(req_msg.board_id !=this->curr_board_id)
    {
     info =tr("类型不符合，升级结束！");
     emit UpgradeOutput(info);
     return;
    }
    UpgradeRespone_Msg res_msg;
    res_msg.board_id = this->curr_board_id;
    res_msg.fw_size = this->curr_fw_size;
    res_msg.sw_ver = this->fw_version;
    res_msg.build_utc = this->build_utc;
    Communication::Instance()->SendData(this->DeviceIP,res_msg.toByteArray());
    }
    break;
    case CMD_UPGRADE_INFO:
    {
      this->update_step = 2;
      UpgradeRespone_Msg msg;
      msg.fromByteArray(in_arr);
      if(msg.board_id!=this->curr_board_id)
      {
          return;

      }
      if(msg.payload[0] == static_cast<char>(0x00))
      {
       info = tr("收到设备NACK应答，升级结束！");
       emit UpgradeOutput(info);
       return;
      }
       this->current_pack = 0;
       write_fw_data(this->current_pack);
       info = tr("发送升级包...");
       emit UpgradeOutput(info);
    }

        break;
    case CMD_UPGRADE_PACK:
    {
        this->update_step = 3;
        UpgradeData_Msg msg;
        msg.fromByteArray(in_arr);

        if(msg.board_id!=this->curr_board_id)
        {
            return;
        }
       this->retry =0;
       this->current_pack++;
        process_cal();
       if(msg.pack_index != this->current_pack)
       {
       this->current_pack = msg.pack_index;
       write_fw_data(this->current_pack);
       }else
       {
       write_fw_data(this->current_pack);
       }
    }
        break;
    case CMD_UPGRADE_FINISH:
    {
       UpgradeData_Msg msg;
       msg.fromByteArray(in_arr);

       if(msg.board_id!=this->curr_board_id)
        {
            return;
        }
      if(this->update_step != 3)
      {
          return;
      }
     if(msg.payload[0]==(char)(0x01)) //升级成功
     {
         {
           this->start = 0;
           this->retry = 0;
           emit UpgradeProgress(100);
           emit UpgradeFinish(UPGRADERESULT_SUCESS);
           //emit UpgradeOutput(tr("升级成功"));
         }
     }else
     {
        this->start = 0;
        this->retry = 0;
        emit UpgradeProgress(0);
        emit UpgradeFinish(UPGRADERESULT_FAIL);
       // emit UpgradeOutput(tr("升级失败"));
     }

    }
        break;
    default:
        break;
    }

}
