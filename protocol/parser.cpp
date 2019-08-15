#include "parser.h"

#include <QtCore/QDebug>


LCSParser::LCSParser(QObject *parent)
    : QObject(parent)
{
    this->state = STATE_WAIT_SYNC1;
}

void LCSParser::ParseData(QHostAddress &ip,QByteArray &data)
{
    for(int i = 0; i < data.size();i++)
    {
        this->ParseData(ip,(quint8)data[i]);
    }
}
void LCSParser::ParseData(QHostAddress &ip,quint8 ch)
{
    switch(state)
    {
    case STATE_WAIT_SYNC1:
        if(ch==(quint8)FRAME_SYNC1)
        {
            data_buffer.clear();
            data_buffer.append(ch);
            state = STATE_WAIT_SYNC2;
        }
        break;
    case STATE_WAIT_SYNC2:
        if(ch==(quint8)FRAME_SYNC2)
        {
            data_buffer.append(ch);
            state = STATE_GET_HEAD;
        }else
        {
            state = STATE_WAIT_SYNC1;
        }
        break;
    case STATE_GET_HEAD:
        data_buffer.append(ch);
        if(data_buffer.size() >=6)
        {
            data_length = (data_buffer[4]&0xff) + (data_buffer[5]&0xff)*256;
            if(data_length <= 1024)
            {
                if(data_length==0)
                {
                    state = STATE_GET_CHECKSUM;
                }else
                {
                    state = STATE_GET_PAYLOAD;
                }
            }else
            {
                state = STATE_WAIT_SYNC1;
                qDebug()<< tr("get head error!");
            }
        }
        break;
    case STATE_GET_PAYLOAD:
        data_buffer.append(ch);
        if(data_buffer.size() >= (data_length +6))
        {
            state = STATE_GET_CHECKSUM;
        }
        break;
    case STATE_GET_CHECKSUM:
    {
        quint8 checksum = getCheckSum(data_buffer,data_buffer.size());
        data_buffer.append(ch);
        if(ch != checksum)
        {
            state = STATE_WAIT_SYNC1;
            qDebug()<< tr("checksum error!,checksum=%1,ch=%2").arg(checksum).arg(ch);
            qDebug()<<data_buffer.toHex();
        }else
        {
            state = STATE_GET_END;
        }
    }
        break;
    case STATE_GET_END:
    {
        data_buffer.append(ch);
        if(ch==FRAME_END)
        {
            emit ParseDataReady(ip,data_buffer);
        }else
        {
            qDebug()<< tr("end error!");
        }
        state = STATE_WAIT_SYNC1;
    }
        break;
    default:
        state = STATE_WAIT_SYNC1;
        break;
    }
}
