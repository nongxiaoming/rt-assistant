#include "keystate_msg.h"

KeyState_Msg::KeyState_Msg(QObject *parent)
    : Msg_Base(parent)
{

}
bool KeyState_Msg::fromByteArray(QByteArray arr)
{
    if(Msg_Base::fromByteArray(arr))
    {
        if(this->payload.size()>=60)
        {
         for(int i = 0;i < 60;i++)
         {
            this->key_state[i] = this->payload[i];
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

QByteArray KeyState_Msg::toByteArray()
{
    this->cmd = CMD_KEY_STATE;
    this->payload.clear();
    for(int i = 0;i < 60;i++)
    {
        this->payload.append(this->key_state[i]);
    }
   return Msg_Base::toByteArray();
}
