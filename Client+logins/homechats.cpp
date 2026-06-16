#include "homechats.h"
#include "ui_homechats.h"

HomeChats::HomeChats(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::HomeChats)
{
    ui->setupUi(this);
    connect(Connect::instance(), &Connect::logMessage, this, &HomeChats::slotLogMessage);
}

HomeChats::~HomeChats()
{
    delete ui;
}

void HomeChats::slotLogMessage(QString str)
{
    ui -> textBrowser ->append(str);
}

void HomeChats::on_pushButton_clicked()
{
    Connect::instance() -> SendToServer(ui->lineEdit->text());
    ui -> lineEdit -> clear();
}

void HomeChats::on_lineEdit_returnPressed()
{
    Connect::instance() -> SendToServer(ui->lineEdit->text());
    ui -> lineEdit -> clear();
}