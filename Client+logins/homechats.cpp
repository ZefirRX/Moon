#include "homechats.h"
#include "ui_homechats.h"
#include "QDateTime"

HomeChats::HomeChats(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::HomeChats)
{
    ui->setupUi(this);
    connect(Connect::instance(), &Connect::chatMessageReceived, this, &HomeChats::slotChatMessageReceived);
    connect(Connect::instance(), &Connect::usersListReceived, this, &HomeChats::slotUsersListReceived);
    connect(Connect::instance(), &Connect::privateMessageReceived, this, &HomeChats::slotPrivateMessageReceived);
    connect(Connect::instance(), &Connect::onlineListReceived, this, &HomeChats::slotOnlineListReceived);
    Connect::instance()->sendGetUsers();
    Connect::instance()->sendGetOnline();
}

HomeChats::~HomeChats()
{
    delete ui;
}

void HomeChats::slotChatMessageReceived(QString nickname, QString time, QString text)
{
    ui -> textBrowser -> append("[" + time + "] <b>" + nickname.toHtmlEscaped() + ":</b> " + text.toHtmlEscaped());
}

void HomeChats::slotPrivateMessageReceived(QString nickname, QString time, QString text)
{
    ui -> textBrowser -> append("[ЛС " + time + "] <b>" + nickname.toHtmlEscaped() + ":</b> " + text.toHtmlEscaped());
}

void HomeChats::on_pushButton_clicked()
{
    QString text = ui->lineEdit->text().trimmed();
    if(text.isEmpty())
        return;

    if(currentChat.isEmpty())
        Connect::instance() -> SendMessage(text);
    else
        Connect::instance() -> sendPrivateMessage(currentChat, text);

    ui -> lineEdit -> clear();
}

void HomeChats::on_lineEdit_returnPressed()
{
    on_pushButton_clicked();
}

void HomeChats::slotOnlineListReceived(QStringList nicknames)
{
    for(int i = 1; i < ui->chatList->count(); i++)
    {
        QListWidgetItem *item = ui->chatList->item(i);
        QString name = item->text().replace(" ●", "");

        if(nicknames.contains(name))
        {
            item->setText(name + " ●");
            item->setForeground(QColor("#5A5AF0"));
        }
        else
        {
            item->setText(name);
            item->setForeground(QColor("#888888"));
        }
    }
}

void HomeChats::on_chatList_itemClicked(QListWidgetItem *item)
{
    QString name = item->text().replace(" ●", "");
    currentChat = (name == "Общий чат") ? "" : name;
    ui->chatTitle->setText(name);
}

void HomeChats::slotUsersListReceived(QStringList nicknames)
{
    for(const QString &nick : nicknames)
    {
        ui->chatList->addItem(nick);
    }
}