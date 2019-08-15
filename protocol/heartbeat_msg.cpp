#include "heartbeat_msg.h"

HeartBeat_Msg::HeartBeat_Msg(QObject *parent)
    : Msg_Base(parent)
{
  this->sys_tick = 0;
}

bool HeartBeat_Msg::fromByteArray(QByteArray arr)
{
    if(Msg_Base::fromByteArray(arr))
    {
        if(this->payload.size()>=4)
        {
        //this->sys_tick =
        return true;
        }else
        {
        return false;
        }
    }else
    {
        return false;
    }
}

QByteArray HeartBeat_Msg::toByteArray()
{
   this->payload.clear();
   this->payload.append((char)(this->sys_tick&0xff));
   this->payload.append((char)((this->sys_tick>>8)&0xff));
   this->payload.append((char)((this->sys_tick>>16)&0xff));
   this->payload.append((char)((this->sys_tick>>24)&0xff));

   return Msg_Base::toByteArray();
}
