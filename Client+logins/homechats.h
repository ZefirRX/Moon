#ifndef HOMECHATS_H
#define HOMECHATS_H
#include <QWidget>
#include <QStringList>
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

public slots:
    void slotChatMessageReceived(QString nickname, QString time, QString text);

private slots:
    void slotUsersListReceived(QStringList nickname);
    void on_pushButton_clicked();
    void on_lineEdit_returnPressed();
};

#endif // HOMECHATS_H