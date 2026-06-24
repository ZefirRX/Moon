#ifndef HOMECHATS_H
#define HOMECHATS_H
#include <QWidget>
#include <QStringList>
#include <QListWidgetItem>
#include <QMap>
#include "connect.h"

namespace Ui {
class HomeChats;
}

class HomeChats : public QWidget
{
    Q_OBJECT

public:
    explicit HomeChats(QWidget *parent = nullptr);
    ~HomeChats();

private:
    Ui::HomeChats *ui;
    QString currentChat;
    QMap<QString, QString> chatHistories;
    QString formatMessage(const QString& nick, const QString& time, const QString& text);

public slots:
    void slotChatMessageReceived(QString nickname, QString time, QString text);
    void slotPrivateMessageReceived(QString nickname, QString time, QString text);
    void slotOnlineListReceived(QStringList nicknames);

private slots:
    void slotUsersListReceived(QStringList nickname);
    void on_pushButton_clicked();
    void on_lineEdit_returnPressed();
    void on_chatList_itemClicked(QListWidgetItem *item);
    void slotHistoryReceived(QList<QStringList> messages);
    void slotHistoryPmReceived(QString nick, QList<QStringList> messages);
};

#endif // HOMECHATS_H