#ifndef LOGINWINDOWS_H
#define LOGINWINDOWS_H

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

private slots:
    void on_EditTag_cursorPositionChanged(int arg1, int arg2);

    void on_ButtonRegister_clicked();

private:
    Ui::LoginWindows *ui;
};

#endif // LOGINWINDOWS_H
