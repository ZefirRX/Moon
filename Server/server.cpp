#include "server.h"

server::server()
{
    /*if(this -> listen(QHostAddress::Any, 2323)) // 1. Принимает сигналы с любого адреса 2. Порт прослушки
    {
        qDebug() << "ready to start";
    }
    else
    {
        qDebug() << errorString();

    }*/
    if (listen(QHostAddress::Any, 2323))
    {
        qDebug() << "ready to start";
    }
    else
    {
        qDebug() << "listen error:" << errorString();
    }
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