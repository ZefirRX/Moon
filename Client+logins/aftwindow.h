#ifndef AFTWINDOW_H
#define AFTWINDOW_H
#include "connect.h"
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class aftwindow;
}
QT_END_NAMESPACE

class aftwindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit aftwindow(QWidget *parent = nullptr);
    ~aftwindow() override;
private:
    Ui::aftwindow *ui;
private slots:
    void on_ButtonAftor_clicked();
    void SlotLoginResult(bool ok, QString info);
    void openloginwindow(const QString &link);
};
#endif // AFTWINDOW_H
