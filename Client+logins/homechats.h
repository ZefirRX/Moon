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

private slots:
    void on_pushButton_clicked();
    void on_lineEdit_returnPressed();

private:
    Ui::HomeChats *ui;

public slots:
    void slotLogMessage(QString str);
};

#endif // HOMECHATS_H