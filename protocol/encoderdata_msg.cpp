#include "encoderdata_msg.h"

EndcoderData_Msg::EndcoderData_Msg(QObject *parent)
    : Msg_Base(parent)
{

}

bool EndcoderData_Msg::fromByteArray(QByteArray arr)
{
    if(Msg_Base::fromByteArray(arr))
    {
        if(this->payload.size()>=8)
        {
         for(int i=0;i < 4;i++)
         {
             this->enc_data[i] = (qint16)((this->payload[i*2]&0xff)+(this->payload[i*2+1]&0xff)*256);
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

QByteArray EndcoderData_Msg::toByteArray()
{
    this->payload.clear();
    for(int i=0;i < 4;i++)
    {
        this->payload.append((quint8)(this->enc_data[i]&0xff));
        this->payload.append((quint8)((this->enc_data[i]>>8)&0xff));
    }
   return Msg_Base::toByteArray();
}

qint16 EndcoderData_Msg::getEncoderData(quint8 index)
{
    if(index <4)
    {
    return this->enc_data[index];
    }else
    {
    return 0;
    }
}
