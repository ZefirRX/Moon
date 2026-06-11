#ifndef HOMECHATS_H
#define HOMECHATS_H

#include <QWidget>
#include <QTcpSocket>

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
    void on_ConnectToServer_clicked();

    void on_pushButton_clicked();

    void on_lineEdit_returnPressed();

private:
    Ui::HomeChats *ui;
    QTcpSocket *socket;
    QByteArray Data;
    void SendToServer(QString str);

public slots:
    void slotReadyRead();
};

#endif // HOMECHATS_H
