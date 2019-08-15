#include "network_msg.h"

Network_Msg::Network_Msg(QObject *parent)
    : Msg_Base(parent)
{
   this->cmd = CMD_NETWORK_CONFIG;
}

bool Network_Msg::fromByteArray(QByteArray arr)
{
  Msg_Base::fromByteArray(arr);
  char *pdata = this->payload.data();

  memcpy(&this->mac,&pdata[0],6);
  if(pdata[6] == 0)
  {
  this->dhcp_enable = false;
  }else {
  this->dhcp_enable = true;
  }

  quint32 temp = 0;
  memcpy(&temp,&pdata[7],4);
  this->ip = QHostAddress(temp);
  memcpy(&temp,&pdata[11],4);
  this->gateway = QHostAddress(temp);
  memcpy(&temp,&pdata[15],4);
  this->netmask = QHostAddress(temp);
  memcpy(&temp,&pdata[19],4);
  this->dns = QHostAddress(temp);
  return true;
}

QByteArray Network_Msg::toByteArray()
{
  this->cmd = CMD_NETWORK_CONFIG;
  this->payload.clear();
  this->payload.append((char)(this->mac[0]));
  this->payload.append((char)(this->mac[1]));
  this->payload.append((char)(this->mac[2]));
  this->payload.append((char)(this->mac[3]));
  this->payload.append((char)(this->mac[4]));
  this->payload.append((char)(this->mac[5]));
  if(this->dhcp_enable)
  {
   this->payload.append((char)(0x01));
  }else
  {
  this->payload.append((char)(0x01));
  }
  quint32 temp = this->ip.toIPv4Address();
  this->payload.append((char)(temp&0xff));
  this->payload.append((char)((temp>>8)&0xff));
  this->payload.append((char)((temp>>16)&0xff));
  this->payload.append((char)((temp>>24)&0xff));
  temp = this->gateway.toIPv4Address();
  this->payload.append((char)(temp&0xff));
  this->payload.append((char)((temp>>8)&0xff));
  this->payload.append((char)((temp>>16)&0xff));
  this->payload.append((char)((temp>>24)&0xff));
  temp = this->netmask.toIPv4Address();
  this->payload.append((char)(temp&0xff));
  this->payload.append((char)((temp>>8)&0xff));
  this->payload.append((char)((temp>>16)&0xff));
  this->payload.append((char)((temp>>24)&0xff));
  temp = this->dns.toIPv4Address();
  this->payload.append((char)(temp&0xff));
  this->payload.append((char)((temp>>8)&0xff));
  this->payload.append((char)((temp>>16)&0xff));
  this->payload.append((char)((temp>>24)&0xff));
  return Msg_Base::toByteArray();
}

