#include "homechats.h"
#include "ui_homechats.h"
#include "QDateTime"
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

void HomeChats::slotChatMessageReceived(QString nickname, QString time, QString text)
{
    ui -> textBrowser -> append("[" + time + "] <b>" + nickname.toHtmlEscaped() + ":</b> " + text.toHtmlEscaped());
}

void HomeChats::on_pushButton_clicked()
{
    QString text = ui->lineEdit->text().trimmed();
    if(text.isEmpty())
        return;

    Connect::instance() -> SendMessage(text);
    ui -> lineEdit -> clear();
}

void HomeChats::on_lineEdit_returnPressed()
{
    QString text = ui->lineEdit->text().trimmed();
    if(text.isEmpty())
        return;

    Connect::instance() -> SendMessage(text);
    ui -> lineEdit -> clear();
}