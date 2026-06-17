#include "homechats.h"
#include "ui_homechats.h"

HomeChats::HomeChats(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::HomeChats)
{
    ui->setupUi(this);
    connect(Connect::instance(), &Connect::chatMessageReceived, this, &HomeChats::slotChatMessageReceived);
}

HomeChats::~HomeChats()
{
    delete ui;
}

void HomeChats::slotChatMessageReceived(QString text)
{
    qDebug() << "HomeChats получил:" << text;
    ui -> textBrowser ->append(text);
}

void HomeChats::on_pushButton_clicked()
{
    Connect::instance() -> SendMessage(ui->lineEdit->text());
    ui -> lineEdit -> clear();
}

void HomeChats::on_lineEdit_returnPressed()
{
    Connect::instance() -> SendMessage(ui->lineEdit->text());
    ui -> lineEdit -> clear();
}