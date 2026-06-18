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

void Connect::findServer()
{
    QByteArray data = "MOON_DISCOVER";
    udpSocket->writeDatagram(data, QHostAddress::Broadcast, 45454);
    emit logMessage("Поиск сервера...");
}

void Connect::SendToServer(QString str)
{
    Data.clear();
    QDataStream out(&Data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_9);
    out << str;
    socket -> write(Data);
}

void Connect::SendMessage(QString text)
{
    SendToServer("MSG|" + text);
}

void Connect::SendRegister(QString nickname, QString tag, QString password)
{
    SendToServer("REG|" + nickname + "|" + tag + "|" + password);
}

void Connect::SendLogin(QString tag, QString password)
{
    SendToServer("LOGIN|" + tag + "|" + password);
}

\

void Connect::slotReadyRead()
{

    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_6_9);
    if(in.status()==QDataStream::Ok)
    {
        QString str;
        in >> str;
        qDebug() << "Connect получил:" << str;
        int sep = str.indexOf('|');
        QString commandStr = (sep == -1) ? str : str.left(sep);
        QString rest = (sep == -1) ? "" : str.mid(sep + 1);

        // Создаем и заполняем простой словарь строк и чисел
        // статический (static), чтобы не пересоздавать его при каждом вызове

        static const QHash<QString, int> commandMap = {
            {"MSG",        CmdMsg},
            {"LOGIN_OK",   CmdLoginOk},
            {"LOGIN_FAIL", CmdLoginFail},
            {"REG_OK",     CmdRegOk},
            {"REG_FAIL",   CmdRegFail}
        };

        //  Ищем команду в словаре. Если не нашли вернули -1
        int code = commandMap.value(commandStr, -1);

        // Вызов сигналов
        switch (code)
        {
        case CmdMsg: {
            int nameSep = rest.indexOf('|');
            QString nickname = (nameSep == -1) ? "Unknown" : rest.left(nameSep);
            QString afterName = (nameSep == -1) ? "" : rest.mid(nameSep + 1);

            int timeSep = afterName.indexOf('|');
            QString time = (timeSep == -1) ? "" : afterName.left(timeSep);
            QString text = (timeSep == -1) ? afterName : afterName.mid(timeSep + 1);

            emit chatMessageReceived(nickname, time, text);
            break;}
        case CmdLoginOk:   emit loginResult(true, rest); break;
        case CmdLoginFail: emit loginResult(false, ""); break;
        case CmdRegOk:     emit registerResult(true); break;
        case CmdRegFail:   emit registerResult(false); break;
        default:           emit logMessage("Unknown command: " + commandStr); break;
        }
    }
    else
    {
        emit logMessage("read Error");
    }
}