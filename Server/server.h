#ifndef SERVER_H
#define SERVER_H
#include <QTcpServer>
#include <QTcpSocket> //Передача сообщений
#include <QUdpSocket> // Связь с клиентом
#include "database.h"
#include <QVector>

class server : public QTcpServer
{
    Q_OBJECT

public:
    server();
    QTcpSocket *socket;

private:
    QVector <QTcpSocket*> Sockets;
    QUdpSocket *udpSocket;
    QByteArray Data;
    Database database;
    QMap<QTcpSocket*, QString> Nicknames;
    void SendToClient(QString str);
    void SendToOne(QTcpSocket *target, QString str);
    void broadcastOnlineList();

public slots:
    void incomingConnection(qintptr socketDescriptor);
    void slotReadyRead();
    void processDiscovery();
    void slotDisconnected();
};
#endif
