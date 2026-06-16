#include "connect.h"

Connect* Connect::connectInstance = nullptr;

Connect* Connect::instance()
{
    if(connectInstance == nullptr)
        connectInstance = new Connect();
    return connectInstance;
}

Connect::Connect(QObject *parent)
    : QObject(parent)
{
    socket = new QTcpSocket(this);
    udpSocket = new QUdpSocket(this);
    udpSocket -> bind(QHostAddress::Any, 0, QUdpSocket::ShareAddress);
    connect(udpSocket, &QUdpSocket::readyRead, this, &Connect::serverFound);
    connect(socket, &QTcpSocket::readyRead, this, &Connect::slotReadyRead);
    connect(socket, &QTcpSocket::disconnected, socket, &QTcpSocket::deleteLater);
}

void Connect::findServer()
{
    QByteArray data = "MOON_DISCOVER";
    udpSocket->writeDatagram(data, QHostAddress::Broadcast, 45454);
    emit logMessage("Поиск сервера...");
}

void Connect::serverFound()
{
    while(udpSocket->hasPendingDatagrams())
    {
        QByteArray data;
        data.resize(udpSocket->pendingDatagramSize());
        QHostAddress serverIp;
        quint16 port;
        udpSocket->readDatagram(data.data(), data.size(), &serverIp, &port);
        if(data == "MOON_SERVER")
        {
            emit logMessage("Сервер найден: " + serverIp.toString());
            socket->connectToHost(serverIp, 2323);
            return;
        }
    }
}

void Connect::SendToServer(QString str)
{
    Data.clear();
    QDataStream out(&Data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_9);
    out << str;
    socket -> write(Data);
}

void Connect::slotReadyRead()
{
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_6_9);
    if(in.status()==QDataStream::Ok)
    {
        QString str;
        in >> str;
        emit logMessage(str);
    }
    else
    {
        emit logMessage("read Error");
    }
}