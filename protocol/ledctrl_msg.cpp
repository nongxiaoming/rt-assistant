#include "ledctrl_msg.h"

LedCtrl_Msg::LedCtrl_Msg(QObject *parent)
    : Msg_Base(parent)
{
   this->cmd = CMD_LED_CTRL;
}

bool LedCtrl_Msg::fromByteArray(QByteArray arr)
{
  Msg_Base::fromByteArray(arr);
  if(this->payload.size()>=60)
  {
   for(int i= 0;i <60;i++)
   {
     this->led_state[i] = this->payload[i];
   }
  }
  return true;
}

QByteArray LedCtrl_Msg::toByteArray()
{
  this->cmd = CMD_LED_CTRL;
  this->payload.clear();

  for(int i = 0; i < 60;i++)
  {
     this->payload.append(this->led_state[i]);
  }

  return Msg_Base::toByteArray();
}

bool LedCtrl_Msg::setState(quint8 index ,quint8 state)
{
 if(index >= 60)
 {
  return false;
 }
 led_state[index] = state;

 return true;
}
