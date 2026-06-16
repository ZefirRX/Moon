#ifndef CONNECT_H
#define CONNECT_H

#include <QObject>
#include <QTcpSocket>
#include <QUdpSocket>

class Connect : public QObject
{
    Q_OBJECT

public:
    static Connect* instance();

    QTcpSocket *socket;
    void findServer();
    void SendToServer(QString str);

private:
    explicit Connect(QObject *parent = nullptr);

    QUdpSocket *udpSocket;
    QByteArray Data;
    static Connect *connectInstance;

public slots:
    void slotReadyRead();
    void serverFound();

signals:
    void logMessage(QString str);
};

#endif // CONNECT_H
