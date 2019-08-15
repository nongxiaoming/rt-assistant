#include "protocol.h"

#include <QtCore/QDebug>

quint8 getCheckSum(QByteArray &array,quint32 length)
{
    quint8 checksum = 0;
    quint32 i =0;
    while(length--)
    {
        checksum += array[i++];
    }
    return checksum;
}



