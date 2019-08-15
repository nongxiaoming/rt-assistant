#include "upgraderesponse_msg.h"

UpgradeRespone_Msg::UpgradeRespone_Msg(QObject *parent)
    : Msg_Base(parent)
{
   this->cmd = CMD_UPGRADE_INFO;
}

bool UpgradeRespone_Msg::fromByteArray(QByteArray arr)
{
  Msg_Base::fromByteArray(arr);

  return true;
}

QByteArray UpgradeRespone_Msg::toByteArray()
{
  this->cmd = CMD_UPGRADE_INFO;
  this->payload.clear();
  this->payload.append(this->pip,32);
  this->payload.append((char)(this->sw_ver&0xff));
  this->payload.append((char)((this->sw_ver>>8)&0xff));
  this->payload.append((char)((this->sw_ver>>16)&0xff));
  this->payload.append((char)((this->sw_ver>>24)&0xff));
  this->payload.append((char)(this->build_utc&0xff));
  this->payload.append((char)((this->build_utc>>8)&0xff));
  this->payload.append((char)((this->build_utc>>16)&0xff));
  this->payload.append((char)((this->build_utc>>24)&0xff));
  this->payload.append((char)(this->fw_size&0xff));
  this->payload.append((char)((this->fw_size>>8)&0xff));
  this->payload.append((char)((this->fw_size>>16)&0xff));
  this->payload.append((char)((this->fw_size>>24)&0xff));

  return Msg_Base::toByteArray();
}

