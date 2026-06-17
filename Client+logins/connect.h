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
        void SendLogin(QString tag, QString password);
        void SendRegister(QString nickname, QString tag, QString password);
        void SendMessage(QString text);

private:
    explicit Connect(QObject *parent = nullptr);
    QUdpSocket *udpSocket;
    QByteArray Data;
    static Connect *connectInstance;
    enum CommandCode { CmdMsg, CmdLoginOk, CmdLoginFail, CmdRegOk, CmdRegFail };
        void SendToServer(QString str);

public slots:
    void slotReadyRead();
    void serverFound();

signals:
    void logMessage(QString str);
    void chatMessageReceived(QString text);
    void loginResult(bool ok, QString info);
    void registerResult(bool ok);
};

#endif // CONNECT_H
