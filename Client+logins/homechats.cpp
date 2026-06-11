#include "homechats.h"
#include "ui_homechats.h"

HomeChats::HomeChats(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::HomeChats)
{
    ui->setupUi(this);
    socket = new QTcpSocket(this);
    connect(socket, &QTcpSocket::readyRead, this, &HomeChats::slotReadyRead);
    connect(socket, &QTcpSocket::disconnected, socket, &QTcpSocket::deleteLater);
}

HomeChats::~HomeChats()
{
    delete ui;
}



void HomeChats::on_ConnectToServer_clicked()
{
    socket -> connectToHost("127.0.0.1", 2323);

}

void HomeChats::SendToServer(QString str)
{
    Data.clear();
    QDataStream out(&Data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_9);
    out << str;
    socket -> write(Data);
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

