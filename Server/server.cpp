#include "server.h"
#include "QDateTime"

server::server()
{
    database.open();
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

void server::SendToOne(QTcpSocket *target, QString str)
{
    Data.clear();
    QDataStream out(&Data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_9);
    out << str;
    target -> write(Data);
}

void server::SendToClient(QString str)
{
    Data.clear();
    QDataStream out(&Data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_9);
    out << str;
    for (int i = 0; i < Sockets.size(); i++)
    {
        Sockets[i] -> write(Data);
    }
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
        int sep = str.indexOf('|');
        QString command = (sep == -1) ? str : str.left(sep);
        QString rest = (sep == -1) ? "" : str.mid(sep + 1);

        if(command == "REG")
        {
            QStringList parts = rest.split('|');
            QString nick = parts.value(0);
            QString tag = parts.value(1);
            QString password = parts.value(2);

            if(database.registerUser(nick, tag, password))
            {
                Nicknames[socket] = nick;
                SendToOne(socket, "REG_OK");
            }
            else
                SendToOne(socket, "REG_FAIL");
        }
        else if(command == "LOGIN")
        {
            QStringList parts = rest.split('|');
            QString tag = parts.value(0);
            QString password = parts.value(1);
            QString nick;

            if(database.checkoutLogin(tag, password, nick))
            {
                Nicknames[socket] = nick;
                SendToOne(socket, "LOGIN_OK|" + nick);
            }
            else
                SendToOne(socket, "LOGIN_FAIL");
        }
        else if(command == "MSG")
        {
            QString time = QDateTime::currentDateTime().toString("HH:mm");
            QString nick = Nicknames.value(socket, "Unknown");
            SendToClient("MSG|" + nick + "|" + time + "|" + rest);
        }
    }
    else
    {
        qDebug() << "DataStream Error";
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