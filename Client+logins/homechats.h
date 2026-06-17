#ifndef HOMECHATS_H
#define HOMECHATS_H
#include <QWidget>
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
    void slotChatMessageReceived(QString text);

private slots:
    void on_pushButton_clicked();
    void on_lineEdit_returnPressed();
};

#endif // HOMECHATS_H