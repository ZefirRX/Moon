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
    connect(socket, &QTcpSocket::disconnected, this, &server::slotDisconnected);
    connect(socket, &QTcpSocket::disconnected, socket, &QTcpSocket::deleteLater);
    Sockets.push_back(socket);
    qDebug() << "client connected" << socketDescriptor;
}

void server::slotDisconnected()
{
    QTcpSocket *disconnectedSocket = (QTcpSocket*)sender();

    Sockets.removeAll(disconnectedSocket);
    Nicknames.remove(disconnectedSocket);
    qDebug() << "client disconnected";
    broadcastOnlineList();
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

    while(socket->bytesAvailable() > 0)
    {
        in.startTransaction();
        QString str;
        in >> str;

        if(!in.commitTransaction())
            break;
        if(in.status() == QDataStream::Ok)
        {
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
                    broadcastOnlineList();
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
                    broadcastOnlineList();
                }
                else
                    SendToOne(socket, "LOGIN_FAIL");
            }
            else if(command == "MSG")
            {
                QString time = QDateTime::currentDateTime().toString("HH:mm");
                QString nick = Nicknames.value(socket, "Unknown");
                database.saveMessage(nick, "", rest, time);
                SendToClient("MSG|" + nick + "|" + time + "|" + rest);
            }
            else if(command == "GET_USERS")
            {
                QStringList nicknames = database.getAllNicknames();
                SendToOne(socket, "USERS|" + nicknames.join(","));
            }
            else if(command == "PM")
            {
                int sep2 = rest.indexOf('|');
                QString receiverNick = (sep2 == -1) ? rest : rest.left(sep2);
                QString text = (sep2 == -1) ? "" : rest.mid(sep2 + 1);

                QTcpSocket *receiverSocket = Nicknames.key(receiverNick, nullptr);

                if(receiverSocket != nullptr)
                {
                    QString time = QDateTime::currentDateTime().toString("HH:mm");
                    QString fromNick = Nicknames.value(socket, "Unknown");

                    database.saveMessage(fromNick, receiverNick, text, time);

                    // получатель
                    SendToOne(receiverSocket,
                              "PM_IN|" + fromNick + "|" + time + "|" + text);

                    // отправитель
                    SendToOne(socket,
                              "PM_OUT|" + receiverNick + "|" + time + "|" + text);
                }
                else
                {
                    SendToOne(socket, "PM_FAIL|" + receiverNick);
                }
            }
            else if(command == "GET_ONLINE")
            {
                QStringList online = Nicknames.values();
                SendToOne(socket, "ONLINE|" + online.join(","));
            }
            else if(command == "GET_HISTORY")
            {
                QString nick = Nicknames.value(socket, "");
                QList<QStringList> history = database.getHistory(nick, "");

                QString result;
                for(const QStringList& row : history)
                {
                    result += row[0] + "~" + row[1] + "~" + row[2] + ";";
                }
                SendToOne(socket, "HISTORY|" + result);
            }
            else if(command == "GET_HISTORY_PM")
            {
                QString myNick = Nicknames.value(socket, "");
                QList<QStringList> history = database.getHistory(myNick, rest);

                QString result;
                for(const QStringList& row : history)
                {
                    result += row[0] + "~" + row[1] + "~" + row[2] + ";";
                }
                SendToOne(socket, "HISTORY_PM|" + rest + "|" + result);
            }
        }
        else
        {
            qDebug() << "DataStream Error";
        }
    }

}

void server::broadcastOnlineList()
{
    QStringList online = Nicknames.values();
    SendToClient("ONLINE|" + online.join(","));
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