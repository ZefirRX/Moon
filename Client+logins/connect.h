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
        void sendGetUsers();
        void sendGetOnline();
        void sendPrivateMessage(QString receiver, QString text);
        void sendGetHistory();
        void sendGetHistoryPm(QString nick);
private:
    explicit Connect(QObject *parent = nullptr);
    QUdpSocket *udpSocket;
    QByteArray Data;
    static Connect *connectInstance;
    enum CommandCode { CmdMsg, CmdLoginOk, CmdLoginFail, CmdRegOk, CmdRegFail, CmdUsers, CmdPm, CmdPmFail, CmdOnline, CmdHistory,
                       CmdHistoryPm };
    void SendToServer(QString str);

public slots:
    void slotReadyRead();
    void serverFound();

signals:
    void logMessage(QString str);
    void chatMessageReceived(QString nickname, QString time, QString text);
    void loginResult(bool ok, QString info);
    void registerResult(bool ok);
    void usersListReceived(QStringList nicknames);
    void privateMessageReceived(QString nickname, QString time, QString text);
    void onlineListReceived(QStringList nicknames);
    void historyReceived(QList<QStringList> messages);
    void historyPmReceived(QString nick, QList<QStringList> messages);
};

#endif // CONNECT_H
