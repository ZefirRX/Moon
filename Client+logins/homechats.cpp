#include "homechats.h"
#include "ui_homechats.h"
#include "QDateTime"

/**
 * @brief Класс для управления подключением к серверу.
 *
 * Отвечает за установку соединения, отправку запросов
 * и получение ответов от удалённого сервера.
 */
HomeChats::HomeChats(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::HomeChats)
{
    ui->setupUi(this);
    connect(Connect::instance(), &Connect::chatMessageReceived, this, &HomeChats::slotChatMessageReceived);
    connect(Connect::instance(), &Connect::usersListReceived, this, &HomeChats::slotUsersListReceived);
    connect(Connect::instance(), &Connect::privateMessageReceived, this, &HomeChats::slotPrivateMessageReceived);
    connect(Connect::instance(), &Connect::onlineListReceived, this, &HomeChats::slotOnlineListReceived);
    connect(Connect::instance(), &Connect::historyReceived, this, &HomeChats::slotHistoryReceived);
    connect(Connect::instance(), &Connect::historyPmReceived, this, &HomeChats::slotHistoryPmReceived);
    Connect::instance()->sendGetHistory();
    Connect::instance()->sendGetUsers();
    Connect::instance()->sendGetOnline();
}

QString HomeChats::formatMessage(const QString& nick, const QString& time, const QString& text)
{
    return "[" + time + "] <b>" + nick.toHtmlEscaped() + ":</b> " + text.toHtmlEscaped();
}

HomeChats::~HomeChats()
{
    delete ui;
}

void HomeChats::slotChatMessageReceived(QString nickname, QString time, QString text)
{
    QString line = formatMessage(nickname, time, text) + "<br>";
    chatHistories[""] += line;

    if(currentChat.isEmpty())
        ui->textBrowser->append(line);
}

void HomeChats::slotPrivateMessageReceived(QString nickname, QString time, QString text)
{
    QString line = "[ЛС " + time + "] <b>" + nickname.toHtmlEscaped() + ":</b> " + text.toHtmlEscaped() + "<br>";
    chatHistories[nickname] += line;

    if(currentChat == nickname)
        ui->textBrowser->append(line);
}

void HomeChats::slotHistoryReceived(QList<QStringList> messages)
{
    QString html;
    for(const QStringList& row : messages)
    {
        html += formatMessage(row[0], row[2], row[1]) + "<br>";
    }
    chatHistories[""] = html;

    if(currentChat.isEmpty())
        ui->textBrowser->setHtml(html);
}

void HomeChats::slotHistoryPmReceived(QString nick, QList<QStringList> messages)
{
    QString html;
    for(const QStringList& row : messages)
    {
        html += formatMessage(row[0], row[2], row[1]) + "<br>";
    }
    chatHistories[nick] = html;

    //if(currentChat == nick)
        ui->textBrowser->setHtml(html);
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

    if(chatHistories.contains(currentChat))
    {
        ui->textBrowser->setHtml(chatHistories[currentChat]);
    }
    else
    {
        ui->textBrowser->clear();
        if(!currentChat.isEmpty())
            Connect::instance()->sendGetHistoryPm(currentChat);
    }
}

void HomeChats::slotUsersListReceived(QStringList nicknames)
{
    for(const QString &nick : nicknames)
    {
        ui->chatList->addItem(nick);
    }
}