#include "boardinfo_msg.h"

BoardInfo_Msg::BoardInfo_Msg(QObject *parent)
    : Msg_Base(parent)
{

}

bool BoardInfo_Msg::fromByteArray(QByteArray arr)
{
    if(Msg_Base::fromByteArray(arr))
    {
        if(this->payload.size()>=4)
        {

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

QByteArray BoardInfo_Msg::toByteArray()
{

   return Msg_Base::toByteArray();
}
