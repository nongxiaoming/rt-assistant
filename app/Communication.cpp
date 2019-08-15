#include "Communication.h"

#include <QtCore/QDebug>

Communication *Communication::_instance = 0;

Communication::Communication(QObject *parent)
    : QObject(parent)
{

    this->parser = new LCSParser(this);
    QObject::connect(this->parser,SIGNAL(ParseDataReady(QHostAddress,QByteArray&)),this,SLOT(ParseDataReady(QHostAddress,QByteArray&)));
}
void Communication::init()
{
    QList<QHostAddress> list = QNetworkInterface::allAddresses();
//        QList<QNetworkInterface> itlist=QNetworkInterface::allInterfaces ();
//         foreach(QNetworkInterface it, itlist)
//         {
//           QList<QHostAddress> iplist=  it.allAddresses ();
//           foreach(QHostAddress address, iplist)
//           {
//               qDebug()<<address.toString ();
//           }
//         }
    foreach(QHostAddress address, list)
    {
        if(address.protocol ()==QAbstractSocket::IPv4Protocol){
        QUdpSocket *SearchUDPSocket = new QUdpSocket(this);
         QObject::connect(SearchUDPSocket,SIGNAL(readyRead()),this,SLOT(readyRead()));
         SearchUDPSocket->bind(address,666,QUdpSocket::ShareAddress);
         this->UdpList.append (SearchUDPSocket);
         qDebug()<<address.toString ();
        }
    }

}
int Communication::SendDataBroadcast(QByteArray arr)
{
    foreach(QUdpSocket *SearchUDPSocket, this->UdpList)
    {
    SearchUDPSocket->writeDatagram(arr,arr.size (), QHostAddress::Broadcast,4000);
    }
  return arr.size();
}
int Communication::SendData(QHostAddress &to,QByteArray arr)
{
    foreach(QUdpSocket *SearchUDPSocket, this->UdpList)
    {
    if((SearchUDPSocket->localAddress().toIPv4Address()&0x00ffffff)==(to.toIPv4Address()&0x00ffffff))
    {
     SearchUDPSocket->writeDatagram(arr,arr.size (), to,4000);
     return arr.size();
    }
    }
    if(this->UdpList[0]!=NULL)
    {
        this->UdpList[0]->writeDatagram(arr,arr.size (), to,4000);
        return arr.size();
    }
   return arr.size();
}

void Communication::readyRead()
{
    QByteArray datagram;//用于存放接收的数据报
    QUdpSocket *SearchUDPSocket=static_cast<QUdpSocket *>(sender());
    datagram.resize(SearchUDPSocket->pendingDatagramSize());
    QHostAddress senderAddress;
    quint16 senderPort;
    SearchUDPSocket->readDatagram(datagram.data(), datagram.size(),
                            &senderAddress, &senderPort);
    if(datagram.size() > 0)
    {
        this->parser->ParseData(senderAddress,datagram);
       // qDebug()<<tr("recv:")<< datagram.toHex();
         //this->curr_ip = senderAddress;
    }

}
void Communication::ParseDataReady(QHostAddress ip,QByteArray &data)
{
    this->board_id = data[2];

    //qDebug()<<data.toHex();
    emit MsgReady(this->board_id,ip,data);
}
