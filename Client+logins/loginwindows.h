#ifndef LOGINWINDOWS_H
#define LOGINWINDOWS_H
#include "connect.h"
#include <QDialog>

namespace Ui {
class LoginWindows;
}

class LoginWindows : public QDialog
{
    Q_OBJECT

public:
    explicit LoginWindows(QWidget *parent = nullptr);
    ~LoginWindows();

private:
    Ui::LoginWindows *ui;

private slots:
    void openaftwindow(const QString &link);

private slots:
    void SlotRegisterResualt(bool ok);
    void on_EditTag_cursorPositionChanged(int arg1, int arg2);
    void on_ButtonRegister_clicked();
};

#endif // LOGINWINDOWS_H
