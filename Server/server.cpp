#include "server.h"

server::server()
{
    if (listen(QHostAddress::Any, 2323))
    {
        qDebug() << "ready to start";
    }
    else
    {
        qDebug() << "listen error:" << errorString();
    }
    udpSocket = new QUdpSocket(this);
    udpSocket->bind( QHostAddress::Any, 45454, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);
    connect(udpSocket, &QUdpSocket::readyRead, this, &server::processDiscovery);
}

void server::incomingConnection(qintptr socketDescriptor)
{
    socket = new QTcpSocket;
    socket -> setSocketDescriptor(socketDescriptor);
    connect(socket, &QTcpSocket::readyRead, this, &server::slotReadyRead);
    connect(socket, &QTcpSocket::disconnected, socket, &QTcpSocket::deleteLater);

    Sockets.push_back(socket);
    qDebug() << "client connected" << socketDescriptor;
}

void server::slotReadyRead()
{
    socket = (QTcpSocket*)sender();
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_6_9);
    if(in.status() == QDataStream::Ok)
    {
        qDebug() << "ready...";
        QString str;
        in >> str;
        qDebug() << str;
        SendToClient(str);
    }
    else
    {
        qDebug() << "DataStream Error";
    }
}

void server::SendToClient(QString str)
{
    Data.clear();
    QDataStream out(&Data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_9);
    out << str;
    //socket -> write(Data);
    for (int i = 0; i < Sockets.size(); i++)
    {
        Sockets[i] -> write(Data);
    }
}

void server::processDiscovery()
{
    while (udpSocket->hasPendingDatagrams())
    {
        QByteArray data;
        data.resize(udpSocket->pendingDatagramSize());

        QHostAddress sender;
        quint16 senderPort;

        udpSocket->readDatagram(
            data.data(),
            data.size(),
            &sender,
            &senderPort);

        qDebug() << "UDP:" << data;

        if(data == "MOON_DISCOVER")
        {
            QByteArray answer = "MOON_SERVER";

            udpSocket->writeDatagram(
                answer,
                sender,
                senderPort);

            qDebug() << "Server found request from" << sender;
        }
    }
}