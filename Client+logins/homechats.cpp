#include "homechats.h"
#include "ui_homechats.h"

HomeChats::HomeChats(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::HomeChats)
{
    ui->setupUi(this);
    socket = new QTcpSocket(this);
    udpSocket = new QUdpSocket(this);
    udpSocket -> bind(QHostAddress::Any, 0, QUdpSocket::ShareAddress);
    connect(udpSocket, &QUdpSocket::readyRead, this, &HomeChats::serverFound);
    connect(socket, &QTcpSocket::readyRead, this, &HomeChats::slotReadyRead);
    connect(socket, &QTcpSocket::disconnected, socket, &QTcpSocket::deleteLater);
}

HomeChats::~HomeChats()
{
    delete ui;
}

void HomeChats::findServer()
{
    QByteArray data = "MOON_DISCOVER";
    udpSocket->writeDatagram(data, QHostAddress::Broadcast, 45454);
    ui->textBrowser->append("Поиск сервера...");
}

void HomeChats::serverFound()
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
            ui->textBrowser->append("Сервер найден: " + serverIp.toString());
            socket->connectToHost(serverIp, 2323);
            return;
        }
    }
}


void HomeChats::on_ConnectToServer_clicked()
{
    findServer();
}

void HomeChats::SendToServer(QString str)
{
    Data.clear();
    QDataStream out(&Data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_9);
    out << str;
    socket -> write(Data);
    ui -> lineEdit -> clear();
}

void HomeChats::slotReadyRead()
{
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_6_9);
    if(in.status()==QDataStream::Ok)
    {
        QString str;
        in >> str;
        ui -> textBrowser ->append(str);
    }
    else
    {
        ui->textBrowser ->append("read Error");
    }
}


void HomeChats::on_pushButton_clicked()
{
    SendToServer(ui->lineEdit->text());
}


void HomeChats::on_lineEdit_returnPressed()
{
    SendToServer(ui->lineEdit->text());
}

