#include "upgradedata_msg.h"

UpgradeData_Msg::UpgradeData_Msg(QObject *parent)
    : Msg_Base(parent)
{
   this->cmd = CMD_UPGRADE_PACK;
}

bool UpgradeData_Msg::fromByteArray(QByteArray arr)
{
  Msg_Base::fromByteArray(arr);
  if(this->payload.size()>=2)
  {
  this->pack_index = (this->payload[0]&0xff) + (this->payload[1]&0xff)*256;
  }
  return true;
}

QByteArray UpgradeData_Msg::toByteArray()
{
  this->cmd = CMD_UPGRADE_PACK;

  return Msg_Base::toByteArray();
}
