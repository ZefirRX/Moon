#ifndef SERVER_H
#define SERVER_H
#include <QTcpServer>
#include <QTcpSocket> //Передача сообщений
#include <QUdpSocket> // Связь с клиентом
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
    void SendToClient(QString str);

public slots:
    void incomingConnection(qintptr socketDescriptor);
    void slotReadyRead();
    void processDiscovery();
};
#endif
