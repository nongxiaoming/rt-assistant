#include "faderdata_msg.h"

FaderData_Msg::FaderData_Msg(QObject *parent)
    : Msg_Base(parent)
{

}

bool FaderData_Msg::fromByteArray(QByteArray arr)
{
    if(Msg_Base::fromByteArray(arr))
    {
        if(this->payload.size()>=30)
        {
         for(int i = 0;i<15;i++)
         {
          this->fader_data[i] = ((this->payload[i*2]&0xff)+(this->payload[i*2+1]&0xff)*256);
         }
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

QByteArray FaderData_Msg::toByteArray()
{
    this->payload.clear();
    for(int i=0;i < 15;i++)
    {
        this->payload.append((quint8)(this->fader_data[i]&0xff));
        this->payload.append((quint8)((this->fader_data[i]>>8)&0xff));
    }
   return Msg_Base::toByteArray();
}
