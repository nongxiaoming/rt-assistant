#include "upgraderequest_msg.h"

UpgradeRequest_Msg::UpgradeRequest_Msg(QObject *parent)
    : Msg_Base(parent)
{
   this->cmd = CMD_UPGRADE_REQ;
}

bool UpgradeRequest_Msg::fromByteArray(QByteArray arr)
{
  Msg_Base::fromByteArray(arr);
  char *pdata = this->payload.data();
  memcpy(&this->type,&pdata[0],1);
  memcpy(&this->oem_code,&pdata[1],2);
  memcpy(this->dev_id,&pdata[3],12);
  memcpy(this->pip,&pdata[15],32);
  memcpy(&this->sw_ver,&pdata[47],4);
  memcpy(&this->build_utc,&pdata[51],4);
  return true;
}

QByteArray UpgradeRequest_Msg::toByteArray()
{
  this->cmd = CMD_UPGRADE_REQ;
  this->payload.clear();
  this->payload.append((char)(this->type&0xff));
  this->payload.append((char)(this->oem_code&0xff));
  this->payload.append((char)((this->oem_code>>8)&0xff));
  this->payload.append((char)(this->sw_ver&0xff));
  this->payload.append((char)((this->sw_ver>>8)&0xff));
  this->payload.append((char)((this->sw_ver>>16)&0xff));
  this->payload.append((char)((this->sw_ver>>24)&0xff));
  this->payload.append((char)(this->build_utc&0xff));
  this->payload.append((char)((this->build_utc>>8)&0xff));
  this->payload.append((char)((this->build_utc>>16)&0xff));
  this->payload.append((char)((this->build_utc>>24)&0xff));
  return Msg_Base::toByteArray();
}

