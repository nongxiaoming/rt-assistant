#include "discovery_msg.h"

Discovery_Msg::Discovery_Msg(QObject *parent)
    : Msg_Base(parent)
{
   this->cmd = CMD_DISCOVERY;
}

bool Discovery_Msg::fromByteArray(QByteArray arr1)
{
  Msg_Base::fromByteArray(arr1);

  QByteArray arr = this->payload.mid(0,20);

  if(this->payload.size() >= 40)
  {
  this->Name = QString(arr);

  this->Name = this->Name.trimmed();

  arr = this->payload.mid(20,20);

  this->SN =  QString(arr);

  this->SN = this->SN.trimmed();
  }
  if(this->payload.size() >= 54)
  {
    char *pdata = this->payload.data();
    memcpy(&this->mac,&pdata[40],6);
    memcpy(&this->sw_ver,&pdata[46],4);
    memcpy(&this->build_date,&pdata[50],4);
  }
  return true;
}

QByteArray Discovery_Msg::toByteArray()
{
  this->cmd = CMD_DISCOVERY;
  this->payload.clear();

//  QByteArray arr = this->Name.trimmed().toLocal8Bit();
//   if(arr.size() >20)
//   {

//   }

  return Msg_Base::toByteArray();
}


